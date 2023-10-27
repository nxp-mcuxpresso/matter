// Copyright 2023 NXP
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "client_http.hpp"
#include "server_http.hpp"
#include <future>

#include "config/asio_no_tls.hpp"
#include "server.hpp"

// Added for the json-example
#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

// Added for the default_resource example
#include <algorithm>
#include <boost/filesystem.hpp>
#include <fstream>
#include <vector>
#ifdef HAVE_OPENSSL
#include "crypto.hpp"
#endif

#include <fstream>
#include <iostream>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <mutex>

#include "commands/common/Commands.h"
#include "commands/example/ExampleCredentialIssuerCommands.h"

#include "commands/clusters/ReportCommand.h"
#include "commands/discover/Commands.h"
#include "commands/group/Commands.h"
#include "commands/interactive/Commands.h"
#include "commands/pairing/Commands.h"
#include "commands/payload/Commands.h"
#include "commands/storage/Commands.h"

#include <lib/support/logging/CHIPLogging.h>
#include <zap-generated/cluster/Commands.h>

#include <controller/ExamplePersistentStorage.h>

using namespace std;
// Added for the json-example:
using namespace boost::property_tree;
// Define a global mutex
std::mutex g_mutex;
std::mutex sq_mutex;

#define RESPONSE_SUCCESS "successful"
#define RESPONSE_FAILURE "failed"

using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;
using HttpClient = SimpleWeb::Client<SimpleWeb::HTTP>;

//typedef websocketpp::server<websocketpp::config::asio> WsServer;
using WsServer = websocketpp::server<websocketpp::config::asio>;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
typedef WsServer::message_ptr message_ptr;
//using namespace WsServer::message_ptr;

// Added for the default_resource example
void default_resource_send(const HttpServer & server, const shared_ptr<HttpServer::Response> & response,
                           const shared_ptr<ifstream> & ifs);

Commands commands;
bool inited = false;
ExampleCredentialIssuerCommands credIssuerCommands;
PersistentStorage webCommissionerStorage;
int chipToolInit()
{
    if (inited)
        return 0;

    inited = true;
    registerCommandsDiscover(commands, &credIssuerCommands);
    registerCommandsInteractive(commands, &credIssuerCommands);
    registerCommandsPayload(commands);
    registerCommandsPairing(commands, &credIssuerCommands);
    registerCommandsGroup(commands, &credIssuerCommands);
    registerClusters(commands, &credIssuerCommands);
    registerCommandsStorage(commands);
    ChipLogError(NotSpecified, "chipToolInit successfuly");
}

int chipToolInteractiveCommand(const char* cmd)
{
    // Lock the critical section with a lock_guard
    std::lock_guard<std::mutex> lock(g_mutex);

    std::ofstream fifo("/tmp/webui_fifo", std::ios::app);
    fifo << cmd << std::endl;
    fifo.flush();
    fifo.close();

    std::ifstream fifo2("/tmp/webui_ack_fifo");
    std::string line;
    if (std::getline(fifo2, line))
    {
        std::cout << line << std::endl;
        ChipLogError(NotSpecified, "thread read ack fifo %s", line.c_str());
    }
    if (fifo2.eof())
    {
        fifo2.clear();
    }
    fifo2.close();
    if (line == "0")
        return 0;
    else
        return 1;
}

string exec_cmd(string cmd) {
    char buffer[128];
    string result = "";
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        cerr << "Error: popen() failed!" << endl;
        exit(1);
    }
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);
    return result;
}

void enableFifo()
{
    if (boost::filesystem::exists("/tmp/webui_fifo"))
    {
        boost::filesystem::remove("/tmp/webui_fifo");
    }
    std::system("mkfifo /tmp/webui_fifo");

    if (boost::filesystem::exists("/tmp/webui_ack_fifo"))
    {
        boost::filesystem::remove("/tmp/webui_ack_fifo");
    }
    std::system("mkfifo /tmp/webui_ack_fifo");

    ChipLogError(NotSpecified, "enable fifo");

    while (true)
    {
        char * args[] = { "chip-tool", "interactive", "start" };
        int ret       = commands.Run(3, args);
        ChipLogError(NotSpecified, "One interactive loop done!!!");
        if (ret)
        {
            ChipLogError(NotSpecified, "interactive return error %d", ret);
            break;
        }
    }
}

ptree getStorageKeyNodeID()
{
    ptree storageNodes;
    const char * storageWebDirectory = webCommissionerStorage.GetDirectory();
    std::string storageWebFile = std::string(storageWebDirectory) + "/chip_tool_config.web.ini";
    std::ifstream ifs(storageWebFile, std::ios::in);
    if (!ifs.is_open())
    {
        ChipLogError(NotSpecified, "Failed to open storage file chip_tool_config.web.ini.");
        return ptree();
    }
    std::string line;
    std::getline(ifs, line);
    while(std::getline(ifs, line))
    {
        size_t equalsPos = line.find('=');
        if (equalsPos != std::string::npos)
        {
            std::string storageNodeAlias = line.substr(0, equalsPos);
            chip::NodeId storageNodeId =  webCommissionerStorage.GetLocalKeyNodeId(storageNodeAlias.c_str());
            storageNodes.put(storageNodeAlias.c_str(), static_cast<int>(storageNodeId));
        }
    }
    ifs.close();

    ChipLogError(NotSpecified, "Get web local storage node alias and nodeID successfully.");
    return storageNodes;
}

void generateMessages(WsServer* s, websocketpp::connection_hdl hdl, message_ptr msg)
{
    string report_text;
    ptree storageNodes = getStorageKeyNodeID();
    while (true)
    {
        std::lock_guard<std::mutex> lock(sq_mutex);
        if (!SubscribeBuffers::IsQueueEmpty())
        {
            //std::lock_guard<std::mutex> lock(sq_mutex);
            ChipReport r = SubscribeBuffers::DequeueReport();
            stringstream report_ss;
            string nodeAlias;
            for (const auto& pair : storageNodes)
            {
                if (pair.second.get_value<int>() == r.nodeid)
                {
                    nodeAlias = pair.first;
                    break;
                }
            }
            report_ss << "Subscribe Report from " << nodeAlias << " " << r.nodeid << ": " << r.endpoint << ". "
                      << "Cluster: " << r.cluster << "\r\n\r\n" << r.attr << ": " << r.value;
            report_text = report_ss.str();
            try
            {
                s->send(hdl, report_text, msg->get_opcode());
                ChipLogError(NotSpecified, "WebSocket server send subscribe report msg: %s", report_text.c_str());
            }
            catch (websocketpp::exception const & e)
            {
                ChipLogError(NotSpecified, "WebSocket server send subscribe report msg failed because %s", e.what());
            }
        }
        sleep(1);
    }
}

// Define a callback to handle incoming messages
void on_message(WsServer* s, websocketpp::connection_hdl hdl, message_ptr msg)
{
    ChipLogError(NotSpecified, "WebSocket server receive msg: %s", msg->get_payload());
    std::string command;
    command = msg->get_payload();
    if (chipToolInteractiveCommand(command.c_str())) {
        ChipLogError(NotSpecified, "subscribe cmd: %s execute failed!", command.c_str());
    } else {
        ChipLogError(NotSpecified, "subscribe cmd: %s execute successfully!", command.c_str());
    }
    std::thread generator(generateMessages, s, hdl, msg);
    generator.detach();
}

int main()
{
    // HTTP-server at port 8889 using 1 thread
    // Unless you do more heavy non-threaded processing in the resources,
    // 1 thread is usually faster than several threads
    chipToolInit();
    webCommissionerStorage.Init("web");
    HttpServer server;
    server.config.port = 8889;
    // Initialize and execute a child thread
    std::thread t(enableFifo);

    // Add resources using path-regex and method-string, and an anonymous function
    // POST-example for the path /string, responds the posted string
    server.resource["^/string$"]["POST"] = [](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        // Retrieve string:
        auto content = request->content.string();
        // request->content.string() is a convenience function for:
        // stringstream ss;
        // ss << request->content.rdbuf();
        // auto content=ss.str();

        //*response << "HTTP/1.1 200 OK\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
        // Alternatively, use one of the convenience functions, for instance:
        response->write(content);
    };

    // POST-example for the path /json, responds firstName+" "+lastName from the posted json
    // Responds with an appropriate error message if the posted json is not valid, or if firstName or lastName is missing
    // Example posted json:
    //{
    //   "firstName": "John",
    //   "lastName": "Smith",
    //   "age": 25
    // }
    server.resource["^/json$"]["POST"] = [](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        try
        {
            ptree pt;
            read_json(request->content, pt);

            auto name=pt.get<string>("firstName")+" "+pt.get<string>("lastName");
            // *response << "HTTP/1.1 200 OK\r\n"
            //           << "Content-Type: application/json\r\n"
            //           << "Content-Length: " << name.length() << "\r\n\r\n"
            //           << name;
            // Alternatively, using a convenience function:
            response->write(name);
        }
        catch(const exception &e)
        {
            //*response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << strlen(e.what()) << "\r\n\r\n" << e.what();
            response->write(SimpleWeb::StatusCode::client_error_bad_request, e.what());
        }
    };

    // GET-example for the path /info
    // Responds with request-information
    server.resource["^/info$"]["GET"] = [](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        stringstream stream;
        stream << "<h1>Request from " << request->remote_endpoint().address().to_string() << ":" << request->remote_endpoint().port() << "</h1>";
        stream << request->method << " " << request->path << " HTTP/" << request->http_version;
        stream << "<h2>Query Fields</h2>";
        auto query_fields = request->parse_query_string();
        for(auto &field : query_fields)
            stream << field.first << ": " << field.second << "<br>";

        stream << "<h2>Header Fields</h2>";
        for(auto &field : request->header)
            stream << field.first << ": " << field.second << "<br>";

        response->write(stream);
    };

    // GET-example for the path /match/[number], responds with the matched string in path (number)
    // For instance a request GET /match/123 will receive: 123
    server.resource["^/match/([0-9]+)$"]["GET"] = [](shared_ptr<HttpServer::Response> response,
                                                            shared_ptr<HttpServer::Request> request) {
        ChipLogError(NotSpecified, "Received GET request for resource at path: %s", request->path_match[1].str().c_str());
        response->write(request->path_match[1].str());
    };

    server.resource["^/pairing$"]["POST"] = [](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        try
        {
            ptree root;
            read_json(request->content, root);
            auto nodeId  = root.get<string>("nodeId");
            auto pinCode = root.get<string>("pinCode");
            auto type  = root.get<string>("type");
            auto nodeAlias = root.get<string>("nodeAlias");
            if(webCommissionerStorage.SyncDoesKeyExist(nodeAlias.c_str())){
                root.put("result", RESPONSE_FAILURE);
                root.put("cause", "repeat nodeAlias");
            } else {
                ChipLogError(NotSpecified, "Received POST request for pairing with Node ID: %s, PIN Code: %s, Type: %s",
                nodeId.c_str(), pinCode.c_str(), type.c_str());
                std::string command;
                if (type == "onnetwork") {
                    command = "pairing onnetwork " + nodeId + " " + pinCode;
                } else if (type == "ble-wifi") {
                    auto ssId = root.get<string>("ssId");
                    auto password = root.get<string>("password");
                    auto discriminator = root.get<string>("discriminator");
                    command = "pairing ble-wifi " + nodeId + " " + ssId + " " + password + " " + pinCode + " " + discriminator;
                } else if (type == "ble-thread") {
                    auto dataset = root.get<string>("dataset");
                    auto discriminator = root.get<string>("discriminator");
                    command = "pairing ble-thread " + nodeId + " hex:" + dataset + " " + pinCode + " " + discriminator;
                }
                // Get the time before executing the command
                auto start_time = std::chrono::steady_clock::now();
                if (chipToolInteractiveCommand(command.c_str())) {
                    root.put("result", RESPONSE_FAILURE);
                } else {
                    root.put("result", RESPONSE_SUCCESS);
                    chip::NodeId nodeIdStorage = std::stoul(nodeId);
                    const char * nodeAliasStorage = nodeAlias.c_str();
                    webCommissionerStorage.SetLocalKeyNodeId(nodeAliasStorage, nodeIdStorage);
                }
                // Get the end time of the command and calculate the difference with the start time (in seconds)
                auto end_time = std::chrono::steady_clock::now();
                auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();
                // Set the value in root according to the timeout: 60s
                if (elapsed_seconds > 60) {
                    root.put("result", RESPONSE_FAILURE);
                }
            }
            stringstream ss;
            write_json(ss, root);
            string strContent = ss.str();
            response->write(strContent);
        } catch (const exception & e)
        {
            response->write(SimpleWeb::StatusCode::client_error_bad_request, e.what());
        }
    };
    server.resource["^/get_dataset$"]["GET"] = [](shared_ptr<HttpServer::Response> response,
                                                  shared_ptr<HttpServer::Request> /*request*/) {
        try
        {
            // Use exec_cmd function to execute OTBR command，and get the OpDataset
            string output = exec_cmd("ot-ctl dataset active -x");

            // Get OpDataset form the output
            string opdataset = "";
            size_t pos = output.find("\r\nDone");
            if (pos != string::npos) {
                opdataset = output.substr(0, pos);
            }

            // Serialize OpDataset to ptree
            ptree root;
            if (opdataset != "") {
                root.put("result", RESPONSE_SUCCESS);
                root.put("message", "OpDataset obtained");
                root.put("dataset", opdataset);
            } else {
                root.put("result", RESPONSE_FAILURE);
                root.put("message", "OpDataset not found");
            }

            // Serialize ptree to string and send to client
            stringstream ss;
            write_json(ss, root);
            string strContent = ss.str();
            response->write(strContent);
        } catch (const exception &e)
        {
            response->write(SimpleWeb::StatusCode::client_error_bad_request, e.what());
        }
    };
    server.resource["^/get_report$"]["POST"] = [](shared_ptr<HttpServer::Response> response,
                                                  shared_ptr<HttpServer::Request> request) {
        try
        {
            ptree root;
            read_json(request->content, root);
            ChipLogError(NotSpecified, "Received POST request for generating report");
            string report_text;
            if (!ReportBuffers::IsQueueEmpty())
            {
                ChipReport r = ReportBuffers::DequeueReport();
                stringstream report_ss;
                ptree storageNodes = getStorageKeyNodeID();
                string nodeAlias;
                for (const auto& pair : storageNodes)
                {
                    if (pair.second.get_value<int>() == r.nodeid)
                    {
                        nodeAlias = pair.first;
                        break;
                    }
                }
                report_ss << "Report from " << nodeAlias << " " << r.nodeid << ": " << r.endpoint << ". "
                                << "Cluster: " << r.cluster << "\r\n\r\n" << r.attr << ": " << r.value;
                report_text = report_ss.str();
                root.put("result", RESPONSE_SUCCESS);
                ChipLogError(NotSpecified, "Generated report successfully: %s", report_text.c_str());
            } else {
                std::thread([](){
                    //std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }).detach();
            }
            root.put("report", report_text);
            stringstream ss;
            write_json(ss, root);
            string strContent = ss.str();
            response->write(strContent);
        } catch (const exception & e)
        {
            response->write(SimpleWeb::StatusCode::client_error_bad_request, e.what());
        }
    };
    server.resource["^/onoff$"]["POST"] = [](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        try
        {
            ptree root;
            read_json(request->content, root);
            auto nodeId  = root.get<string>("nodeId");
            auto endPointId = root.get<int>("endPointId");
            auto type  = root.get<string>("type");
            const map<string, string> typeToVerb = {
                {"on", "switch on"},
                {"off", "switch off"},
                {"toggle", "toggle"},
                {"read", "read the status of"}
            };
            string verb = typeToVerb.count(type) ? typeToVerb.at(type) : "perform an operation on";
            ChipLogError(NotSpecified, "Received POST request to %s the device", verb.c_str());
            std::string command;
            if (type == "on") {
                command = "onoff on " + nodeId + " " + std::to_string(endPointId);
            } else if (type == "off") {
                command = "onoff off " + nodeId + " " + std::to_string(endPointId);
            } else if (type == "toggle") {
                command = "onoff toggle " + nodeId + " " + std::to_string(endPointId);
            } else if (type == "read") {
                string attr  = root.get<string>("attr");
                command = "onoff read on-off " + nodeId + " " + std::to_string(endPointId);
            }
            auto start_time = std::chrono::steady_clock::now();
            if (chipToolInteractiveCommand(command.c_str())) {
                root.put("result", RESPONSE_FAILURE);
            } else {
                root.put("result", RESPONSE_SUCCESS);
            }

            auto end_time = std::chrono::steady_clock::now();
            auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();
            if (elapsed_seconds > 60) {
                root.put("result", RESPONSE_FAILURE);
            }
            stringstream ss;
            write_json(ss, root);
            string strContent = ss.str();
            response->write(strContent);
        } catch (const exception & e)
        {
            response->write(SimpleWeb::StatusCode::client_error_bad_request, e.what());
        }
    };
    server.resource["^/multiadmin$"]["POST"] = [](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request)
    {
        try
        {
            ptree root;
            read_json(request->content, root);
            auto nodeId  = root.get<string>("nodeId");
            auto option = root.get<string>("option");
            auto windowTimeout = root.get<int>("windowTimeout");
            auto iteration  = root.get<string>("iteration");
            auto discriminator  = root.get<string>("discriminator");
            ChipLogError(NotSpecified, "Received POST request to open commissioning window");
            std::string command;
            command = "pairing open-commissioning-window " + nodeId + " " + option + " " + std::to_string(windowTimeout) + " " + iteration + " " + discriminator;
            auto start_time = std::chrono::steady_clock::now();
            if (chipToolInteractiveCommand(command.c_str())) {
                root.put("result", RESPONSE_FAILURE);
            } else {
                root.put("result", RESPONSE_SUCCESS);
            }

            auto end_time = std::chrono::steady_clock::now();
            auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();
            if (elapsed_seconds > 60) {
                root.put("result", RESPONSE_FAILURE);
            }
            stringstream ss;
            write_json(ss, root);
            string strContent = ss.str();
            response->write(strContent);
        } catch (const exception &e)
        {
            response->write(SimpleWeb::StatusCode::client_error_bad_request, e.what());
        }
    };

    server.resource["^/get_status$"]["GET"] = [](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request)
    {
        try
        {
            ptree root;
            ptree storageNodes;
            ChipLogError(NotSpecified, "Received GET request for get status");
            auto start_time = std::chrono::steady_clock::now();
            try{
                storageNodes = getStorageKeyNodeID();
                root.put("result", RESPONSE_SUCCESS);
                root.add_child("status", storageNodes);
            } catch (const exception & e)
            {
                ChipLogError(NotSpecified, "GET request for get status failed");
                root.put("result", RESPONSE_FAILURE);
            }
            auto end_time = std::chrono::steady_clock::now();
            auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();
            if (elapsed_seconds > 60) {
                root.put("result", RESPONSE_FAILURE);
            }
            stringstream ss;
            write_json(ss, root);
            string strContent = ss.str();
            response->write(strContent);
        } catch (const exception & e)
        {
            response->write(SimpleWeb::StatusCode::client_error_bad_request, e.what());
        }
    };

    server.resource["^/delete_storageNode$"]["POST"] = [](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request)
    {
        try
        {
            ptree root;
            read_json(request->content, root);
            auto nodeAlias  = root.get<string>("nodeAlias");
            ChipLogError(NotSpecified, "Received POST request to delete storage node with node alias: %s", nodeAlias.c_str());
            auto start_time = std::chrono::steady_clock::now();
            try
            {
                webCommissionerStorage.SyncDeleteKeyValue(nodeAlias.c_str());
                root.put("result", RESPONSE_SUCCESS);
            } catch (const exception & e)
            {
                ChipLogError(NotSpecified, "Delete storage node with nodeAlias: %s failed", nodeAlias.c_str());
                root.put("result", RESPONSE_FAILURE);
            }
            auto end_time = std::chrono::steady_clock::now();
            auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();
            if (elapsed_seconds > 60) {
                root.put("result", RESPONSE_FAILURE);
            }
            stringstream ss;
            write_json(ss, root);
            string strContent = ss.str();
            response->write(strContent);
        } catch (const exception &e)
        {
            response->write(SimpleWeb::StatusCode::client_error_bad_request, e.what());
        }
    };

    // Get example simulating heavy work in a separate thread
    server.resource["^/work$"]["GET"] = [&server](shared_ptr<HttpServer::Response> response,
                                                  shared_ptr<HttpServer::Request> /*request*/) {
        thread work_thread([response] {
            // this_thread::sleep_for(chrono::seconds(5));
            std::ofstream fifo("/tmp/webui_fifo", std::ios::app);
            fifo << "pairing onnetwork 1 20202021" << std::endl;
            fifo.flush();
            string message = "Work done";
            //*response << "HTTP/1.1 200 OK\r\nContent-Length: " << message.length() << "\r\n\r\n" << message;
            response->write(message);
        });
        work_thread.detach();
    };

    server.resource["^/getAvailableNetwork$"]["GET"] = [&server](shared_ptr<HttpServer::Response> response,
                                                                 shared_ptr<HttpServer::Request> /*request*/) {
        thread work_thread([response] {
            //this_thread::sleep_for(chrono::seconds(5));
            // string message="Work done";
            string json_string = "{\"result\":[{\"ch\":13,\"ha\":\"18B43000003D2785\",\"nn\":\"NEST-PAN-C1E7\",\"pi\":\"0xC19B\","
                                 "\"xp\":\"EEA74CE1EDFA2E8A\"}]}";
            // string json_string="{\"firstName\": \"John\",\"lastName\": \"Smith\",\"age\": 25}";
            // *response << "HTTP/1.1 200 OK\r\nContent-Length: " << json_string.length()
            //           << "\r\nContent-Type:application/json; charset=utf-8"
            //           << "\r\n\r\n"
            //           << json_string;
            response->write(json_string);
        });
        work_thread.detach();
    };

    // Default GET-example. If no other matches, this anonymous function will be called.
    // Will respond with content in the web/-directory, and its subdirectories.
    // Default file: index.html
    // Can for instance be used to retrieve an HTML 5 client that uses REST-resources on this server
    char* frontend_path = getenv("CHIP_TOOL_WEB_FRONTEND");
    if (frontend_path == nullptr) {
        frontend_path = "/usr/share/chip-tool-web/frontend";
    }
    server.default_resource["GET"] = [&server, frontend_path](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        try
        {
            auto web_root_path = boost::filesystem::canonical(frontend_path);
            auto path          = boost::filesystem::canonical(web_root_path / request->path);
            // Check if path is within web_root_path
            if (distance(web_root_path.begin(), web_root_path.end()) > distance(path.begin(), path.end()) ||
                !equal(web_root_path.begin(), web_root_path.end(), path.begin()))
                throw invalid_argument("path must be within root path");
            if (boost::filesystem::is_directory(path))
                path /= "index.html";
            if (!(boost::filesystem::exists(path) && boost::filesystem::is_regular_file(path)))
                throw invalid_argument("file does not exist");

            SimpleWeb::CaseInsensitiveMultimap header;

            // Uncomment the following line to enable Cache-Control
            // header.emplace("Cache-Control", "max-age=86400");

#ifdef HAVE_OPENSSL
    // Uncomment the following lines to enable ETag
    // {
    //     ifstream ifs(path.string(), ifstream::in | ios::binary);
    //     if(ifs) {
    //         auto hash=SimpleWeb::Crypto::to_hex_string(SimpleWeb::Crypto::md5(ifs));
    //         header.emplace("ETag", "\"" + hash + "\"");
    //         auto it=request->header.find("If-None-Match");
    //         if(it!=request->header.end()) {
    //             if(!it->second.empty() && it->second.compare(1, hash.size(), hash)==0) {
    //                 response->write(SimpleWeb::StatusCode::redirection_not_modified, header);
    //                 return;
    //             }
    //         }
    //     }
    //     else
    //         throw invalid_argument("could not read file");
    // }
#endif

            auto ifs = make_shared<ifstream>();
            ifs->open(path.string(), ifstream::in | ios::binary | ios::ate);

            if (*ifs)
            {
                auto length = ifs->tellg();
                ifs->seekg(0, ios::beg);

                header.emplace("Content-Length", to_string(length));
                response->write(header);

                // Trick to define a recursive function within this scope (for example purposes)
                class FileServer {
                public:
                    static void read_and_send(const shared_ptr<HttpServer::Response> &response, const shared_ptr<ifstream> &ifs) {
                        // Read and send 128 KB at a time
                        static vector<char> buffer(131072); // Safe when server is running on one thread
                        streamsize read_length;
                        if((read_length = ifs->read(&buffer[0], static_cast<streamsize>(buffer.size())).gcount()) > 0) {
                            response->write(&buffer[0], read_length);
                            if(read_length == static_cast<streamsize>(buffer.size())) {
                                response->send([response, ifs](const SimpleWeb::error_code &ec) {
                                if(!ec)
                                    read_and_send(response, ifs);
                                else
                                    cerr << "Connection interrupted" << endl;
                                });
                            }
                        }
                    }
                };
                FileServer::read_and_send(response, ifs);
            }
            else
                throw invalid_argument("could not read file");
        } catch (const exception &e) {
            response->write(SimpleWeb::StatusCode::client_error_bad_request, "Could not open path " + request->path + ": " + e.what());
        }
    };

    server.on_error = [](shared_ptr<HttpServer::Request> /*request*/, const SimpleWeb::error_code & /*ec*/) {
        // Handle errors here
        // Note that connection timeouts will also call this handle with ec set to SimpleWeb::errc::operation_canceled
    };

    // Start server and receive assigned port when server is listening for requests
    promise<unsigned short> server_port;
    thread server_thread([&server, &server_port]() {
        // Start server
        server.start([&server_port](unsigned short port) {
            server_port.set_value(port);
        });
    });
    //cout << "Server listening on port " << server_port.get_future().get() << endl;

    // Wait for server to start so that the client can connect
    this_thread::sleep_for(chrono::seconds(1));

    // Client examples
    // string json_string = "{\"firstName\": \"John\",\"lastName\": \"Smith\",\"age\": 25}";

    // Synchronous request examples
    // {
    //     HttpClient client("localhost:8080");
    //     try {
    //     cout << "Example GET request to http://localhost:8080/match/123" << endl;
    //     auto r1 = client.request("GET", "/match/123");
    //     cout << "Response content: " << r1->content.rdbuf() << endl // Alternatively, use the convenience function r1->content.string()
    //         << endl;

    //     cout << "Example POST request to http://localhost:8080/string" << endl;
    //     auto r2 = client.request("POST", "/string", json_string);
    //     cout << "Response content: " << r2->content.rdbuf() << endl
    //         << endl;
    //     }
    //     catch(const SimpleWeb::system_error &e) {
    //     cerr << "Client request error: " << e.what() << endl;
    //     }
    // }

    // Asynchronous request example
    // {
    //     HttpClient client("localhost:8080");
    //     cout << "Example POST request to http://localhost:8080/json" << endl;
    //     client.request("POST", "/json", json_string, [](shared_ptr<HttpClient::Response> response, const SimpleWeb::error_code &ec) {
    //         if(!ec)
    //         cout << "Response content: " << response->content.rdbuf() << endl;
    //     });
    //     client.io_service->run();
    // }

    // Create a websocket server endpoint
    WsServer wsserver;

    try {
        // Set logging settings
        wsserver.set_access_channels(websocketpp::log::alevel::all);
        wsserver.clear_access_channels(websocketpp::log::alevel::frame_payload);

        // Initialize Asio
        wsserver.init_asio();

        // Register our message handler
        wsserver.set_message_handler(bind(&on_message,&wsserver,::_1,::_2));

        // Listen on port 9002
        wsserver.listen(9002);

        // Start the server accept loop
        wsserver.start_accept();

        // Start the ASIO io_service run loop
        wsserver.run();

    }
    catch (websocketpp::exception const & e)
    {
        ChipLogError(NotSpecified, "WebSocket connection disconnected because: %s", e.what());
    }
    catch (...)
    {
        ChipLogError(NotSpecified, "WebSocket connection disconnected because other exception");
    }

    server_thread.join();
    t.join(); // Wait for the child thread to end

    return 0;
}
