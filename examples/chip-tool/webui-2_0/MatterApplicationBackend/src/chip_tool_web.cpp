// Copyright 2024 NXP
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

#include <future>

#include "config/asio_no_tls.hpp"
#include "server.hpp"
#include "WebSocketClient.h"

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

// Crow web server
#include <crow/http_request.h>
#include <crow/http_response.h>
#include <crow/json.h>
#include <crow/app.h>
#include "crow/middlewares/cors.h"

// For file reading by the server
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

using namespace std;
// Added for the json-example:
using namespace boost::property_tree;

#define RESPONSE_SUCCESS "successful"
#define RESPONSE_FAILURE "failed"

//typedef websocketpp::server<websocketpp::config::asio> WsServer;
using WsServer = websocketpp::server<websocketpp::config::asio>;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
typedef WsServer::message_ptr message_ptr;
//using namespace WsServer::message_ptr;

ExampleCredentialIssuerCommands credIssuerCommands;
PersistentStorage webCommissionerStorage;

Commands commands;
bool initialized = false;

#ifndef CROW_FRONTEND_LOCATION
#define CROW_STATIC_DIRECTORY "/usr/share/chip-tool-web/frontend2/"
#elif defined(CROW_FRONTEND_LOCATION)
#define CROW_STATIC_DIRECTORY CROW_FRONTEND_LOCATION
#endif

WebSocketClient wsClient;
int chipToolInit()
{
    if (initialized)
        return 0;

    initialized = true;
    registerCommandsDiscover(commands, &credIssuerCommands);
    registerCommandsInteractive(commands, &credIssuerCommands);
    registerCommandsPayload(commands);
    registerCommandsPairing(commands, &credIssuerCommands);
    registerCommandsGroup(commands, &credIssuerCommands);
    registerClusters(commands, &credIssuerCommands);
    registerCommandsStorage(commands);
    ChipLogError(NotSpecified, "chipToolInit successfuly");
    return 0;
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

void enableChipServer()
{
    while (true)
    {
        char * args[] = { "chip-tool", "interactive", "server", "--port", "9008" };
        ChipLogError(NotSpecified, "starting the chip-tool interactive servr.");
        int ret       = commands.Run(5, args);
        ChipLogError(NotSpecified, "One interactive loop done!!!");
        if (ret)
        {
            ChipLogError(NotSpecified, "interactive return error %d", ret);
            break;
        }
    }
}

void wsClientConecting()
{
    while (true)
    {
        std::string wsconnetion = wsClient.connection_staus();
        if (wsconnetion != "Closed"){
            ChipLogError(NotSpecified, "The connection between wsClient and chip-tool interactive wsServer is work well.");
        }
        else {
            ChipLogError(NotSpecified, "The connection between wsClient and chip-tool interactive wsServer disconnect and retry connect.");
            wsClient.connect("ws://localhost:9008");
        }
        this_thread::sleep_for(chrono::seconds(10));
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

void generateMessages(WsServer* s, websocketpp::connection_hdl hdl, message_ptr msg, string nodeId, string nodeAlias)
{
    string report_text;
    while (true)
    {
        if(!subscribeReportQueue.empty())
        {
            Json::Value resultsValue = wsClient.dequeueSubscribeReport();
            Json::Value arryValue = resultsValue[0];
            stringstream report_ss;
            report_ss << "Subscribe Report from " << nodeAlias << " " << nodeId << ": " << arryValue["endpointId"] << ". " << "Cluster: "
                    << arryValue["clusterId"] << "\r\n\r\n" << "On-Off" << ": " << arryValue["value"];
            report_text = report_ss.str();
            ChipLogError(NotSpecified, "Receive subscribe message: %s", report_text.c_str());
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
        this_thread::sleep_for(chrono::seconds(5));
    }
}

// Define a callback to handle incoming messages
void on_message(WsServer* s, websocketpp::connection_hdl hdl, message_ptr msg)
{
    ChipLogError(NotSpecified, "WebSocket server for subscription receive msg: %s", msg->get_payload().c_str());
    std::string command;
    command = msg->get_payload();
    Json::Value jsonObject;
    Json::Reader reader;
    std::string minInterval, maxInterval, nodeId, endPointId, nodeAlias;
    ChipLogError(NotSpecified, "Received WebSockets request for subscribe with command: %s", command.c_str());
    if (reader.parse(command, jsonObject))
    {
        minInterval  = jsonObject["minInterval"].asString();
        maxInterval  = jsonObject["maxInterval"].asString();
        nodeAlias = jsonObject["nodeAlias"].asString();
        nodeId = jsonObject["nodeId"].asString();
        endPointId = jsonObject["endPointId"].asString();
    }
    ChipLogError(NotSpecified, "Received WebSockets request for subscribe with Node ID: %s, Endpoint ID: %s", nodeId.c_str(), endPointId.c_str());
    std::string subscribeCommand;
    subscribeCommand = "onoff subscribe on-off " + minInterval + " " + maxInterval + " " + nodeId + " " + endPointId;
    wsClient.sendMessage(subscribeCommand);
    ChipLogError(NotSpecified, "Send subscribe command to chip-tool ws server.");
    int sleepTime = 0;
    while (reportQueue.empty() && sleepTime < 20)
    {
        this_thread::sleep_for(chrono::seconds(1));
        sleepTime++;
    }
    if (sleepTime == 20) {
        ChipLogError(NotSpecified, "Receive subscribe command to chip-tool ws server overtime!");
    } else {
        Json::Value resultsValue = wsClient.dequeueReport();
        Json::Value arryValue = resultsValue[0];
        stringstream report_ss;
        string report_text;
        ChipLogError(NotSpecified, "Received subscribe report from chip-tool ws server: %s", arryValue.toStyledString().c_str());
        report_ss << "Subscribe Report from " << nodeAlias << " " << nodeId << ": " << arryValue["endpointId"] << ". " << "Cluster: "
                << arryValue["clusterId"] << "\r\n\r\n" << "On-Off" << ": " << arryValue["value"];
        report_text = report_ss.str();
        ChipLogError(NotSpecified, "Generated report successfully: %s", report_text.c_str());
        try
        {
            s->send(hdl, report_text, msg->get_opcode());
            ChipLogError(NotSpecified, "WebSocket server send subscribe report msg: %s", report_text.c_str());
        }
        catch (websocketpp::exception const & e)
        {
            ChipLogError(NotSpecified, "WebSocket server send subscribe report msg failed because %s", e.what());
        }
        std::thread generator(generateMessages, s, hdl, msg, nodeId, nodeAlias);
        generator.detach();
    }
}

bool validate(WsServer* s, websocketpp::connection_hdl connection) {
    s->get_con_from_hdl(connection)->append_header("Access-Control-Allow-Origin", "*");
    s->get_con_from_hdl(connection)->append_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    s->get_con_from_hdl(connection)->append_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
    s->get_con_from_hdl(connection)->append_header("Access-Control-Allow-Credentials", "true");
    return true;
}

// sockjs emulation
void websocket_on_http(WsServer* s, websocketpp::connection_hdl hdl) {
	auto con = s->get_con_from_hdl(hdl);
	con->set_status(websocketpp::http::status_code::ok);
	con->append_header("access-control-allow-origin", "*");
	con->append_header("content-type", "application/json; charset=UTF-8");
	ptree body;
    body.put("websocket", true);
    body.put("origins", "*:*");
    body.put("cookie_needed", false);
    std::ostringstream oss;
    write_json(oss, body);
    std::string body_str = oss.str();
    con->set_body(body_str);
}

//////////////// Serve the angular web content
void MatterServerApplicationBlueprint(crow::Blueprint& bp) {
    CROW_BP_ROUTE(bp, "static/")([](const crow::request& req, crow::response& res) {
        crow::mustache::context ctx;
        res.code = 200;
        // res.add_header("Content-Type", "text/html");
        // std::ifstream ifs(std::string(CROW_STATIC_DIRECTORY) + "index.html");
        std::ifstream ifs("index.html");
        std::string fileContent;
        fileContent.assign(std::istreambuf_iterator<char>(ifs),
                            std::istreambuf_iterator<char>());
        res.write(fileContent);
        res.end();
    });
}

void setContentTypeForFileName(crow::response& res, std::string fileName) {
    if (fileName.find(".html") != std::string::npos) {
        res.add_header("Content-Type", "text/html");
    } else if (fileName.find(".css") != std::string::npos) {
        res.add_header("Content-Type", "text/css");
    } else if (fileName.find(".js") != std::string::npos) {
        res.add_header("Content-Type", "application/javascript");
    } else if (fileName.find(".png") != std::string::npos) {
        res.add_header("Content-Type", "image/png");
    } else if (fileName.find(".jpg") != std::string::npos) {
        res.add_header("Content-Type", "image/jpeg");
    } else if (fileName.find(".ico") != std::string::npos) {
        res.add_header("Content-Type", "image/x-icon");
    } else if (fileName.find(".woff2") != std::string::npos) {
        res.add_header("Content-Type", "font/woff2");
    } else if (fileName.find(".ttf") != std::string::npos) {
        res.add_header("Content-Type", "font/ttf");
    } else {
        res.add_header("Content-Type", "text/plain");
    }
}

void addCrowRouteForStaticFileServer(crow::App<crow::CORSHandler>& crowApplication, std::string path) {
    path.insert(0, "/");
    crowApplication.route_dynamic((const char*)path.c_str())
    ([path](const crow::request& req, crow::response& res) {
        crow::mustache::context ctx;
        res.code = 200;
        setContentTypeForFileName(res, path);
        std::ifstream ifs(std::string(CROW_STATIC_DIRECTORY) + path);
        std::string fileContent;
        std::cout << "GET: Got a file from static file server on path:" << path << "\n";
        fileContent.assign(std::istreambuf_iterator<char>(ifs),
                            std::istreambuf_iterator<char>());
        res.write(fileContent);
        res.end();
    });
}

int main()
{
    chipToolInit();
    webCommissionerStorage.Init("web");
    constexpr int SERVER_PORT = 8889;
    constexpr int WS_PORT = 9002;

    std::thread t(enableChipServer);
    this_thread::sleep_for(chrono::seconds(2));
    wsClient.connect("ws://localhost:9008");
    std::thread wsc(wsClientConecting);

    crow::App<crow::CORSHandler> crowApplication;
    auto& cors = crowApplication.get_middleware<crow::CORSHandler>();

    cors.global()
        .origin("*")
        .methods("GET"_method, "POST"_method, "OPTIONS"_method, "PUT"_method)
        .headers("Content-Type", "Access-Control-Allow-Origin", "Access-Control-Allow-Methods", "Access-Control-Allow-Headers", "Access-Control-Allow-Credentials")
        .allow_credentials();

    crow::mustache::set_global_base(CROW_STATIC_DIRECTORY);

    ChipLogError(NotSpecified, "Using the static directory: %s", CROW_STATIC_DIRECTORY);

    CROW_ROUTE(crowApplication, "/")([](const crow::request& req, crow::response& res) {
        crow::mustache::context ctx;
        res.code = 200;
        res.add_header("Content-Type", "text/html");
        std::ifstream ifs(std::string(CROW_STATIC_DIRECTORY) + "index.html");
        // std::ifstream ifs("index.html");
        std::string fileContent;
        fileContent.assign(std::istreambuf_iterator<char>(ifs),
                            std::istreambuf_iterator<char>());
        res.write(fileContent);
        res.end();
    });

    CROW_ROUTE(crowApplication, "/<string>")
    ([&](const crow::request& req, crow::response& res, std::string path) {
        crow::mustache::context ctx;
        res.code = 200;
        setContentTypeForFileName(res, path);
        if (path == "devices" || path == "dashboard" || path == "subscriptions" || path == "audio" || path == "help"){
            res.set_header("Content-Type", "text/html");
            std::ifstream ifs(std::string(CROW_STATIC_DIRECTORY) + "index.html");
            std::string fileContent;
            fileContent.assign(std::istreambuf_iterator<char>(ifs),
                                std::istreambuf_iterator<char>());
            res.write(fileContent);
            res.end();
        } else {
            std::ifstream ifs(std::string(CROW_STATIC_DIRECTORY) + path);
            std::string fileContent;
            fileContent.assign(std::istreambuf_iterator<char>(ifs),
                                std::istreambuf_iterator<char>());
            res.write(fileContent);
            res.end();
        }
    });

    CROW_ROUTE(crowApplication, "/assets/<string>")
    ([&](const crow::request& req, crow::response& res, std::string path) {
        crow::mustache::context ctx;
        res.code = 200;
        setContentTypeForFileName(res, path);
        std::ifstream ifs(std::string(CROW_STATIC_DIRECTORY) + "assets/" + path);
        std::string fileContent;
        fileContent.assign(std::istreambuf_iterator<char>(ifs),
                            std::istreambuf_iterator<char>());
        res.write(fileContent);
        res.end();
    });

    CROW_ROUTE(crowApplication, "/assets/endpoint/<string>")
    ([&](const crow::request& req, crow::response& res, std::string path) {
        crow::mustache::context ctx;
        res.code = 200;
        setContentTypeForFileName(res, path);
        std::ifstream ifs(std::string(CROW_STATIC_DIRECTORY) + "assets/endpoint/" + path);
        std::string fileContent;
        fileContent.assign(std::istreambuf_iterator<char>(ifs),
                            std::istreambuf_iterator<char>());
        res.write(fileContent);
        res.end();
    });

    CROW_ROUTE(crowApplication, "/assets/matteradvantages/<string>")
    ([&](const crow::request& req, crow::response& res, std::string path) {
        crow::mustache::context ctx;
        res.code = 200;
        setContentTypeForFileName(res, path);
        std::ifstream ifs(std::string(CROW_STATIC_DIRECTORY) + "assets/matteradvantages/" + path);
        std::string fileContent;
        fileContent.assign(std::istreambuf_iterator<char>(ifs),
                            std::istreambuf_iterator<char>());
        res.write(fileContent);
        res.end();
    });

    CROW_ROUTE(crowApplication, "/media/<string>")
    ([&](const crow::request& req, crow::response& res, std::string path) {
        crow::mustache::context ctx;
        res.code = 200;
        setContentTypeForFileName(res, path);
        std::ifstream ifs(std::string(CROW_STATIC_DIRECTORY) + "media/" + path);
        std::string fileContent;
        fileContent.assign(std::istreambuf_iterator<char>(ifs),
                            std::istreambuf_iterator<char>());
        res.write(fileContent);
        res.end();
    });

    CROW_ROUTE(crowApplication, "/api/string").methods("POST"_method)([](const crow::request& req) {
        crow::response response(req.body);
        response.add_header("Access-Control-Allow-Origin", "*");
        return response;
    });

    CROW_ROUTE(crowApplication, "/api/json").methods("POST"_method)([](const crow::request& req) {
        try
        {
            ptree pt;
            read_json(req.body, pt);

            auto name=pt.get<string>("firstName")+" "+pt.get<string>("lastName");
            crow::response response(name);
            response.add_header("Access-Control-Allow-Origin", "*");
            return response;
        }
        catch(const exception &e)
        {
            crow::response response(400, e.what());
            response.add_header("Access-Control-Allow-Origin", "*");
            return response;
        }
    });

    CROW_ROUTE(crowApplication, "/api/info").methods("GET"_method)([](const crow::request& req) {
        std::ostringstream stream;
        ptree pt;
        for(auto &field : req.headers)
            pt.put(field.first, field.second);
        pt.put("Server", "CrowServer");

        stringstream ss;
        write_json(ss, pt);
        string strContent = ss.str();
        crow::response response(strContent);
        return response;
    });

    CROW_ROUTE(crowApplication, "/api/match/<int>").methods("GET"_method)([](int i) {
        crow::response response(to_string(i));
        response.add_header("Access-Control-Allow-Origin", "*");
        return response;
    });

    CROW_ROUTE(crowApplication, "/api/pairing").methods("POST"_method)([](const crow::request& req) {
        try
        {
            ptree root;
            auto x_body_decoded = crow::json::load(req.body);
            auto nodeId = std::string(x_body_decoded["nodeId"].s());
            auto pinCode = std::string(x_body_decoded["pinCode"].s());
            auto type = std::string(x_body_decoded["type"].s());
            auto nodeAlias = std::string(x_body_decoded["nodeAlias"].s());
            ChipLogError(NotSpecified, "Received POST request for pairing with Node ID: %s, PIN Code: %s, Type: %s, Node Alias: %s",
                std::string(nodeId).c_str(), std::string(pinCode).c_str(), std::string(type).c_str(), std::string(nodeAlias).c_str());
            if(webCommissionerStorage.SyncDoesKeyExist(std::string(nodeAlias).c_str())){
                root.put("result", RESPONSE_FAILURE);
                root.put("cause", "repeat nodeAlias");
            } else {
                std::string command;
                if (type == "onnetwork") {
                    command = "pairing onnetwork-commissioning-mode " + nodeId + " " + pinCode;
                } else if (type == "ble-wifi") {
                    auto ssId = std::string(x_body_decoded["ssId"].s());
                    auto password = std::string(x_body_decoded["password"].s());
                    auto discriminator = std::string(x_body_decoded["discriminator"].s());
                    command = "pairing ble-wifi " + nodeId + " " + ssId + " " + password + " " + pinCode + " " + discriminator;
                } else if (type == "ble-thread") {
                    auto dataset = std::string(x_body_decoded["dataset"].s());
                    auto discriminator = std::string(x_body_decoded["discriminator"].s());
                    command = "pairing ble-thread " + nodeId + " hex:" + dataset + " " + pinCode + " " + discriminator;
                }
                wsClient.sendMessage(command);
                ChipLogError(NotSpecified, "Send pairing command to chip-tool ws server.");
                int sleepTime = 0;
                while (reportQueue.empty() && sleepTime < 60)
                {
                    this_thread::sleep_for(chrono::seconds(1));
                    sleepTime++;
                }
                if (sleepTime == 60) {
                    root.put("result", RESPONSE_FAILURE);
                    ChipLogError(NotSpecified, "Execute pairing command overtime!");
                } else {
                    Json::Value resultsReport = wsClient.dequeueReport();
                    int resultsReportSize = resultsReport.size();
                    if (resultsReportSize == 0) {
                        root.put("result", RESPONSE_SUCCESS);
                        chip::NodeId nodeIdStorage = std::stoul(nodeId);
                        const char * nodeAliasStorage = nodeAlias.c_str();
                        webCommissionerStorage.SetLocalKeyNodeId(nodeAliasStorage, nodeIdStorage);
                    } else {
                        root.put("result", RESPONSE_FAILURE);
                        ChipLogError(NotSpecified, "Recieved response meaasge after sending pairing command, but pairing failed!");
                    }
                }
            }
            stringstream ss;
            write_json(ss, root);
            string strContent = ss.str();
            crow::response response(strContent);
            response.add_header("Access-Control-Allow-Origin", "*");
            return response;
        } catch (const exception & e)
        {
            crow::response response(400, e.what());
            response.add_header("Access-Control-Allow-Origin", "*");
            return response;
        }
    });

    CROW_ROUTE(crowApplication, "/api/get_dataset").methods("GET"_method)([]() {
        try
        {
            ChipLogError(NotSpecified, "Received GET request for get OpDataset");
            string output = exec_cmd("ot-ctl dataset active -x");
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
            crow::response response(strContent);
            response.add_header("Access-Control-Allow-Origin", "*");
            return response;
        } catch (const exception &e)
        {
            crow::response response(400, e.what());
            response.add_header("Access-Control-Allow-Origin", "*");
            return response;
        }
    });

    CROW_ROUTE(crowApplication, "/api/onoff_report").methods("POST"_method)([](const crow::request& req) {
        try
        {
            ptree root;
            auto x_body_decoded = crow::json::load(req.body);
            auto nodeAlias = std::string(x_body_decoded["nodeAlias"].s());
            auto nodeId = std::string(x_body_decoded["nodeId"].s());
            auto endPointId = std::string(x_body_decoded["endPointId"].s());
            ChipLogError(NotSpecified, "Received ON/OFF read POST request for device with Node ID: %s, End Point ID: %s",
                std::string(nodeId).c_str(), std::string(endPointId).c_str());
            std::string command;

            // Get the id of the node from the alias
            chip::NodeId nodeIdStorage = webCommissionerStorage.GetLocalKeyNodeId(nodeAlias.c_str());
            if (nodeIdStorage == 0) {
                root.put("result", RESPONSE_FAILURE);
                root.put("cause", "nodeAlias not found");
                ChipLogError(NotSpecified, "Node Alias not found in the storage");
            } else {
              command = "onoff read on-off " + nodeId + " " + endPointId;
              wsClient.sendMessage(command);
              ChipLogError(NotSpecified, "Send onoff read command to chip-tool ws server");
              int sleepTime = 0;
              while (reportQueue.empty() && sleepTime < 20)
              {
                  this_thread::sleep_for(chrono::seconds(1));
                  sleepTime++;
              }
              if (sleepTime == 20) {
                  root.put("result", RESPONSE_FAILURE);
                  ChipLogError(NotSpecified, "Generated onoff report overtime!");
              } else {
                  Json::Value resultsReport = wsClient.dequeueReport();
                  Json::Value resultsValue = resultsReport[0];
                  if (resultsValue.isMember("error"))
                  {
                      root.put("result", RESPONSE_FAILURE);
                      ChipLogError(NotSpecified, "Generated report failed!");
                  } else {
                      stringstream report_ss;
                      report_ss << "Report from " << nodeAlias << " " << nodeId << ": " << resultsValue["endpointId"] << ". " << "Cluster: "
                                << resultsValue["clusterId"] << "; " << "On-Off" << ": " << resultsValue["value"];
                      string report_text = report_ss.str();
                      root.put("result", RESPONSE_SUCCESS);
                      ChipLogError(NotSpecified, "Generated report successfully: %s", report_text.c_str());
                      root.put("report", report_text);
                  }
              }
            }
            stringstream ss;
            write_json(ss, root);
            string strContent = ss.str();
            crow::response response(strContent);
            response.add_header("Access-Control-Allow-Origin", "*");
            return response;
        } catch (const exception & e)
        {
            crow::response response(400, e.what());
            ChipLogError(NotSpecified, "Error on onoff_report: %s", e.what());
            response.add_header("Access-Control-Allow-Origin", "*");
            return response;
        }
    });

    CROW_ROUTE(crowApplication, "/api/onoff").methods("POST"_method)([](const crow::request& req) {
        try
        {
            ptree root;
            auto x_body_decoded = crow::json::load(req.body);
            auto nodeId = std::string(x_body_decoded["nodeId"].s());
            auto endPointId = std::string(x_body_decoded["endPointId"].s());
            auto type = std::string(x_body_decoded["type"].s());
            ChipLogError(NotSpecified, "Received ON/OFF POST request for device with Node ID: %s, End Point ID: %s, Type: %s",
                std::string(nodeId).c_str(), std::string(endPointId).c_str(), std::string(type).c_str());
            const map<string, string> typeToVerb = {
                {"on", "switch on"},
                {"off", "switch off"},
                {"toggle", "toggle"},
            };
            string verb = typeToVerb.count(type) ? typeToVerb.at(type) : "perform an operation on";
            ChipLogError(NotSpecified, "Received POST request to %s the device", verb.c_str());
            std::string command;
            if (type == "on") {
                command = "onoff on " + nodeId + " " + endPointId;
            } else if (type == "off") {
                command = "onoff off " + nodeId + " " + endPointId;
            } else if (type == "toggle") {
                command = "onoff toggle " + nodeId + " " + endPointId;
            }
            wsClient.sendMessage(command);
            ChipLogError(NotSpecified, "send onoff command to chip-tool ws server");
            int sleepTime = 0;
            while (reportQueue.empty() && sleepTime < 20)
            {
                this_thread::sleep_for(chrono::seconds(1));
                sleepTime++;
            }
            if (sleepTime == 20) {
                root.put("result", RESPONSE_FAILURE);
                ChipLogError(NotSpecified, "Execute onoff command overtime!");
            } else {
                Json::Value resultsValue = wsClient.dequeueReport();
                int jsonObjectsize = resultsValue.size();
                if (jsonObjectsize == 0) {
                    root.put("result", RESPONSE_SUCCESS);
                } else {
                    root.put("result", RESPONSE_FAILURE);
                    ChipLogError(NotSpecified, "Execute onoff command failed!");
                }
            }
            stringstream ss;
            write_json(ss, root);
            string strContent = ss.str();
            crow::response response(strContent);
            response.add_header("Access-Control-Allow-Origin", "*");
            return response;
        } catch (const exception & e)
        {
            crow::response response(400, e.what());
            response.add_header("Access-Control-Allow-Origin", "*");
            return response;
        }
    });

    CROW_ROUTE(crowApplication, "/api/multiadmin").methods("POST"_method)([](const crow::request& req) {
        try
        {
            ptree root;
            auto x_body_decoded = crow::json::load(req.body);
            auto nodeId = std::string(x_body_decoded["nodeId"].s());
            auto option = std::string(x_body_decoded["option"].s());
            auto windowTimeout = std::string(x_body_decoded["windowTimeout"].s());
            auto iteration = std::string(x_body_decoded["iteration"].s());
            auto discriminator = std::string(x_body_decoded["discriminator"].s());
            ChipLogError(NotSpecified, "Received POST request to open commissioning window");
            std::string command;
            command = "pairing open-commissioning-window " + nodeId + " " + option + " " + windowTimeout + " " + iteration + " " + discriminator;
            wsClient.sendMessage(command);
            ChipLogError(NotSpecified, "send multiadmin command to chip-tool ws server");
            int sleepTime = 0;
            while (reportQueue.empty() && sleepTime < 20)
            {
                this_thread::sleep_for(chrono::seconds(1));
                sleepTime++;
            }
            if (sleepTime == 20) {
                root.put("result", RESPONSE_FAILURE);
                ChipLogError(NotSpecified, "Execute multiadmin command overtime!");
            } else {
                Json::Value resultsValue = wsClient.dequeueReport();
                int jsonObjectsize = resultsValue.size();
                if (jsonObjectsize == 0) {
                    root.put("result", RESPONSE_SUCCESS);
                } else {
                    root.put("result", RESPONSE_FAILURE);
                    ChipLogError(NotSpecified, "Execute multiadmin command failed!");
                }
            }
            stringstream ss;
            write_json(ss, root);
            string strContent = ss.str();
            crow::response response(strContent);
            response.add_header("Access-Control-Allow-Origin", "*");
            return response;
        } catch (const exception &e)
        {
            crow::response response(400, e.what());
            response.add_header("Access-Control-Allow-Origin", "*");
            return response;
        }
    });

    CROW_ROUTE(crowApplication, "/api/get_status").methods("GET"_method)([]() {
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
            crow::response response(strContent);
            response.add_header("Access-Control-Allow-Origin", "*");
            return response;
        } catch (const exception & e)
        {
            crow::response response(400, e.what());
            response.add_header("Access-Control-Allow-Origin", "*");
            return response;
        }
    });

    CROW_ROUTE(crowApplication, "/api/delete_storageNode").methods("POST"_method)([](const crow::request& req) {
        try
        {
            ptree root;
            auto x_body_decoded = crow::json::load(req.body);
            auto nodeAlias = std::string(x_body_decoded["nodeAlias"].s());
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
            crow::response response(strContent);
            response.add_header("Access-Control-Allow-Origin", "*");
            return response;
        } catch (const exception &e)
        {
            crow::response response(400, e.what());
            response.add_header("Access-Control-Allow-Origin", "*");
            return response;
        }
    });

    CROW_ROUTE(crowApplication, "/api/write_acl").methods("POST"_method)([](const crow::request& req) {
        try
        {
            ptree root;
            read_json(req.body, root);
            std::array<std::string, 2> aclConfString;
            for(int i = 0; i < 2; i++)
            {
                std::string aclConfKey = "aclConf" + std::to_string(i + 1);
                ptree aclConf = root.get_child(aclConfKey);
                auto fabricIndex = aclConf.get<string>("fabricIndex");
                auto privilege = aclConf.get<string>("privilege");
                auto authMode = aclConf.get<string>("authMode");
                auto subjects = aclConf.get<string>("subjects");
                auto targets = aclConf.get<string>("targets");
                aclConfString[i] = "{\"fabricIndex\": " +  fabricIndex + ", \"privilege\": " + privilege + ", \"authMode\": " + authMode +
                                   ", \"subjects\": [" + subjects + "], \"targets\": " + targets + "}";
            }
            auto lightNodeId = root.get<string>("lightNodeId");
            auto aclEndpointId = root.get<string>("aclEndpointId");
            ChipLogError(NotSpecified, "Received POST request to Write ACL");
            std::string command;
            command = "accesscontrol write acl '[" + aclConfString[0] +"," + aclConfString[1] + "]'" + lightNodeId + " " + aclEndpointId;
            wsClient.sendMessage(command);
            ChipLogError(NotSpecified, "send write_acl command to chip-tool ws server");
            int sleepTime = 0;
            while (reportQueue.empty() && sleepTime < 20)
            {
                this_thread::sleep_for(chrono::seconds(1));
                sleepTime++;
            }
            if (sleepTime == 20) {
                root.put("result", RESPONSE_FAILURE);
                ChipLogError(NotSpecified, "Execute write acl command overtime!");
            } else {
                Json::Value resultsValue = wsClient.dequeueReport();
                int jsonObjectsize = resultsValue.size();
                if (jsonObjectsize == 0) {
                    root.put("result", RESPONSE_SUCCESS);
                } else {
                    root.put("result", RESPONSE_FAILURE);
                    ChipLogError(NotSpecified, "Execute write acl command failed!");
                }
            }
            stringstream ss;
            write_json(ss, root);
            string strContent = ss.str();
            crow::response response(strContent);
            response.add_header("Access-Control-Allow-Origin", "*");
            return response;
        } catch (const exception &e)
        {
            crow::response response(400, e.what());
            response.add_header("Access-Control-Allow-Origin", "*");
            return response;
        }
    });

    CROW_ROUTE(crowApplication, "/api/write_binding").methods("POST"_method)([](const crow::request& req) {
        try
        {
            ptree root;
            read_json(req.body, root);
            ptree bindingConf = root.get_child("bindingConf");
            auto fabricIndex = bindingConf.get<string>("fabricIndex");
            auto node = bindingConf.get<string>("node");
            auto endpoint = bindingConf.get<string>("endPointId");
            auto cluster = bindingConf.get<string>("cluster");
            std::string bindingConfString = "{\"fabricIndex\": " +  fabricIndex + ", \"node\": " + node
                                            + ", \"endpoint\": " + endpoint + ", \"cluster\": " + cluster + "}";
            auto switchNodeId = root.get<string>("switchNodeId");
            auto switchEndpointId = root.get<string>("switchEndpointId");
            ChipLogError(NotSpecified, "Received POST request to Write Binding");
            std::string command;
            command ="binding write binding '[" + bindingConfString + "]'" + switchNodeId + " " + switchEndpointId;
            wsClient.sendMessage(command);
            ChipLogError(NotSpecified, "send write_binding command to chip-tool ws server");
            int sleepTime = 0;
            while (reportQueue.empty() && sleepTime < 20)
            {
                this_thread::sleep_for(chrono::seconds(1));
                sleepTime++;
            }
            if (sleepTime == 20) {
                root.put("result", RESPONSE_FAILURE);
                ChipLogError(NotSpecified, "Execute write binding command overtime!");
            } else {
                Json::Value resultsValue = wsClient.dequeueReport();
                int jsonObjectsize = resultsValue.size();
                if (jsonObjectsize == 0) {
                    root.put("result", RESPONSE_SUCCESS);
                } else {
                    root.put("result", RESPONSE_FAILURE);
                    ChipLogError(NotSpecified, "Execute write binding command failed!");
                }
            }
            stringstream ss;
            write_json(ss, root);
            string strContent = ss.str();
            crow::response response(strContent);
            response.add_header("Access-Control-Allow-Origin", "*");
            return response;
        } catch (const exception &e)
        {
            crow::response response(400, e.what());
            response.add_header("Access-Control-Allow-Origin", "*");
            return response;
        }
    });

    CROW_ROUTE(crowApplication, "/api/launcher").methods("POST"_method)([](const crow::request& req) {
        try
        {
            ptree root;
            read_json(req.body, root);
            auto nodeId = root.get<string>("nodeId");
            auto endPointId = root.get<int>("endPointId");
            auto type = root.get<string>("type");
            auto launchConf = root.get_child("launchConf");
            auto catalogVendorID = launchConf.get<string>("catalogVendorID");
            auto applicationID = launchConf.get<string>("applicationID");
            std::string launchConfString = "'{\"catalogVendorID\": " +  catalogVendorID + ", \"applicationID\": \"" + applicationID + "\"}'";
            std::string command;
            if (type == "launch") {
                command = "applicationlauncher launch-app " + nodeId + " " + std::to_string(endPointId) + " --Application " + launchConfString;
                ChipLogError(NotSpecified, "Received POST request to Launch App");
            } else if (type == "stop") {
                command = "applicationlauncher stop-app " + nodeId + " " + std::to_string(endPointId) + " --Application " + launchConfString;
                ChipLogError(NotSpecified, "Received POST request to Stop App");
            }
            wsClient.sendMessage(command);
            ChipLogError(NotSpecified, "send launcher command to chip-tool ws server");
            int sleepTime = 0;
            while (reportQueue.empty() && sleepTime < 20)
            {
                this_thread::sleep_for(chrono::seconds(1));
                sleepTime++;
            }
            if (sleepTime == 20) {
                root.put("result", RESPONSE_FAILURE);
                ChipLogError(NotSpecified, "Execute launcher command overtime!!");
            } else {
                Json::Value resultsReport = wsClient.dequeueReport();
                Json::Value resultsValue = resultsReport[0];
                if (resultsValue.isMember("error"))
                {
                    root.put("result", RESPONSE_FAILURE);
                    ChipLogError(NotSpecified, "Execute launcher command failed!");
                } else {
                    root.put("result", RESPONSE_SUCCESS);
                    ChipLogError(NotSpecified, "Execute launcher command successfully");
                }
            }
            stringstream ss;
            write_json(ss, root);
            string strContent = ss.str();
            crow::response response(strContent);
            response.add_header("Access-Control-Allow-Origin", "*");
            return response;
        } catch (const exception &e)
        {
            crow::response response(400, e.what());
            response.add_header("Access-Control-Allow-Origin", "*");
            return response;
        }
    });

    CROW_ROUTE(crowApplication, "/api/media_control").methods("POST"_method)([](const crow::request& req) {
        try
        {
            ptree root;
            auto x_body_decoded = crow::json::load(req.body);
            auto nodeId = std::string(x_body_decoded["nodeId"].s());
            auto endPointId = std::string(x_body_decoded["endPointId"].s());
            auto type = std::string(x_body_decoded["type"].s());
            ChipLogError(NotSpecified, "Received POST request for media control" );
            std::string command;
            if (type == "play") {
                command = "mediaplayback play " + nodeId + " " + endPointId;
            } else if (type == "pause") {
                command = "mediaplayback pause " + nodeId + " " + endPointId;
            } else if (type == "stop") {
                command = "mediaplayback stop " + nodeId + " " + endPointId;
            } else if (type == "startover") {
                command = "mediaplayback start-over " + nodeId + " " + endPointId;
            } else if (type == "previous") {
                command = "mediaplayback previous " + nodeId + " " + endPointId;
            } else if (type == "next") {
                command = "mediaplayback next " + nodeId + " " + endPointId;
            } else if (type == "rewind") {
                command = "mediaplayback rewind " + nodeId + " " + endPointId;
            } else if (type == "fastforward") {
                command = "mediaplayback fast-forward " + nodeId + " " + endPointId;
            }
            wsClient.sendMessage(command);
            ChipLogError(NotSpecified, "Send media control command to chip-tool ws server");
            int sleepTime = 0;
            while (reportQueue.empty() && sleepTime < 20)
            {
                this_thread::sleep_for(chrono::seconds(1));
                sleepTime++;
            }
            if (sleepTime == 20) {
                root.put("result", RESPONSE_FAILURE);
                ChipLogError(NotSpecified, "Execute media control command overtime!");
            } else {
                Json::Value resultsReport = wsClient.dequeueReport();
                Json::Value resultsValue = resultsReport[0];
                if (resultsValue.isMember("error"))
                {
                    root.put("result", RESPONSE_FAILURE);
                    ChipLogError(NotSpecified, "Execute media control command failed!");
                } else {
                    root.put("result", RESPONSE_SUCCESS);
                    ChipLogError(NotSpecified, "Execute media control command successfully");
                }
            }
            stringstream ss;
            write_json(ss, root);
            string strContent = ss.str();
            crow::response response(strContent);
            response.add_header("Access-Control-Allow-Origin", "*");
            return response;
        } catch (const exception & e)
        {
            crow::response response(400, e.what());
            response.add_header("Access-Control-Allow-Origin", "*");
            return response;
        }
    });

    CROW_ROUTE(crowApplication, "/api/media_read").methods("POST"_method)([](const crow::request& req) {
        try
        {
            ptree root;
            auto x_body_decoded = crow::json::load(req.body);
            auto nodeId = std::string(x_body_decoded["nodeId"].s());
            auto endPointId = std::string(x_body_decoded["endPointId"].s());
            auto type = std::string(x_body_decoded["type"].s());
            ChipLogError(NotSpecified, "Received POST request for media control" );
            std::string command;
            if (type == "currentstate") {
                command = "mediaplayback read current-state " + nodeId + " " + endPointId;
            } else if (type == "starttime") {
                command = "mediaplayback read start-time " + nodeId + " " + endPointId;
            } else if (type == "duration") {
                command = "mediaplayback read duration " + nodeId + " " + endPointId;
            } else if (type == "sampledposition") {
                command = "mediaplayback read sampled-position " + nodeId + " " + endPointId;
            } else if (type == "playbackspeed") {
                command = "mediaplayback read playback-speed " + nodeId + " " + endPointId;
            }
            wsClient.sendMessage(command);
            ChipLogError(NotSpecified, "Send media read command to chip-tool ws server");
            int sleepTime = 0;
            while (reportQueue.empty() && sleepTime < 46)
            {
                this_thread::sleep_for(chrono::seconds(1));
                sleepTime++;
            }
            if (sleepTime == 46) {
                root.put("result", RESPONSE_FAILURE);
                ChipLogError(NotSpecified, "Execute media read command overtime!");
            } else {
                Json::Value resultsReport = wsClient.dequeueReport();
                Json::Value resultsValue = resultsReport[0];
                if (!resultsValue.isMember("error") && resultsValue.isMember("value") )
                {
                    string report_text;
                    stringstream report_ss;
                    string value;
                    if (type == "currentstate")
                    {
                        switch (resultsValue["value"].asInt())
                        {
                            case 0:
                                value = "Play";
                                break;
                            case 1:
                                value = "Pause";
                                break;
                            case 2:
                                value = "Stop";
                                break;
                            default:
                                value = "Unknown";
                        }
                    } else {
                        value = resultsValue["value"].asString();
                    }
                    report_ss << "Report from " << " " << nodeId << ": " << resultsValue["endPointId"] << ". "
                            << "Cluster: " << resultsValue["clusterId"] << "\r\n\r\n" << type << ": " << value;
                    report_text = report_ss.str();
                    root.put("report", report_text);
                    ChipLogError(NotSpecified, "Generated media app report successfully: %s", report_text.c_str());
                    root.put("result", RESPONSE_SUCCESS);
                } else {
                    root.put("result", RESPONSE_FAILURE);
                    ChipLogError(NotSpecified, "Execute media read command failed!");
                }
            }
            stringstream ss;
            write_json(ss, root);
            string strContent = ss.str();
            crow::response response(strContent);
            response.add_header("Access-Control-Allow-Origin", "*");
            return response;
        } catch (const exception & e)
        {
            crow::response response(400, e.what());
            response.add_header("Access-Control-Allow-Origin", "*");
            return response;
        }
    });

    CROW_ROUTE(crowApplication, "/api/getAvailableNetwork").methods("GET"_method)([&](const crow::request& req) {
        std::thread work_thread([req] {
            //this_thread::sleep_for(chrono::seconds(5));
            // string message="Work done";
            string json_string = "{\"result\":[{\"ch\":13,\"ha\":\"18B43000003D2785\",\"nn\":\"NEST-PAN-C1E7\",\"pi\":\"0xC19B\","
                                 "\"xp\":\"EEA74CE1EDFA2E8A\"}]}";
            // string json_string="{\"firstName\": \"John\",\"lastName\": \"Smith\",\"age\": 25}";
            // *response << "HTTP/1.1 200 OK\r\nContent-Length: " << json_string.length()
            //           << "\r\nContent-Type:application/json; charset=utf-8"
            //           << "\r\n\r\n"
            //           << json_string;
            // response->write(json_string);
        });
        work_thread.detach();
        crow::response response("Work done");
        response.add_header("Access-Control-Allow-Origin", "*");
        return response;
    });

    // Default GET-example. If no other matches, this anonymous function will be called.
    // Will respond with content in the web/-directory, and its subdirectories.
    // Default file: index.html
    // Can for instance be used to retrieve an HTML 5 client that uses REST-resources on this server
    char* frontend_path = getenv("CHIP_TOOL_WEB_FRONTEND");
    if (frontend_path == nullptr) {
        frontend_path = "/home/matthewalgo/Matter2023Q4/examples/chip-tool/webui/frontend";
    }

    CROW_ROUTE(crowApplication, "/api/").methods("GET"_method)([&frontend_path](const crow::request& req) {
        try
        {
            auto web_root_path = boost::filesystem::canonical(frontend_path);
            auto path          = boost::filesystem::canonical(web_root_path / "index.html");
            // Check if path is within web_root_path
            if (distance(web_root_path.begin(), web_root_path.end()) > distance(path.begin(), path.end()) ||
                !equal(web_root_path.begin(), web_root_path.end(), path.begin()))
                throw invalid_argument("Path must be within root path");
            if (boost::filesystem::is_directory(path))
                path /= "index.html";
            if (!(boost::filesystem::exists(path) && boost::filesystem::is_regular_file(path)))
                throw invalid_argument("File does not exist");
            ChipLogError(NotSpecified, "Path to index.html: %s", path.string().c_str());
            std::ifstream ifs(path.string());
            std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
            crow::response response(content);
            response.add_header("Access-Control-Allow-Origin", "*");
            return response;
        } catch (const exception &e) {
            crow::response response(400, e.what());
            response.add_header("Access-Control-Allow-Origin", "*");
            return response;
        }
    });

    CROW_ROUTE(crowApplication, "/api/{resource:.*}").methods("GET"_method)([&frontend_path](const crow::request& req) {
        try
        {
            auto web_root_path = boost::filesystem::canonical(frontend_path);
            auto path          = boost::filesystem::canonical(web_root_path / req.url);
            if (distance(web_root_path.begin(), web_root_path.end()) > distance(path.begin(), path.end()) ||
                !equal(web_root_path.begin(), web_root_path.end(), path.begin()))
                throw invalid_argument("Path must be within root path");
            if (boost::filesystem::is_directory(path))
                path /= "index.html";
            if (!(boost::filesystem::exists(path) && boost::filesystem::is_regular_file(path)))
                throw invalid_argument("File does not exist");
            ChipLogError(NotSpecified, "Path to resource file: %s", path.string().c_str());
            std::ifstream ifs(path.string());
            std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
            crow::response response(content);
            response.add_header("Access-Control-Allow-Origin", "*");
            return response;
        } catch (const exception &e) {
            crow::response response(400, e.what());
            response.add_header("Access-Control-Allow-Origin", "*");
            return response;
        }
    });

    // Start server and receive assigned port when server is listening for requests
    // promise<unsigned short> server_port;
    thread server_thread([&crowApplication ,&SERVER_PORT]() {
        // crow::Blueprint main_bp(std::string(CROW_STATIC_DIRECTORY), std::string(CROW_STATIC_DIRECTORY), std::string(CROW_STATIC_DIRECTORY));
        // MatterServerApplicationBlueprint(main_bp);
        // crowApplication.register_blueprint(main_bp);

        crowApplication.loglevel(crow::LogLevel::Debug);
        crowApplication.port(SERVER_PORT);
        crowApplication.multithreaded().run_async();

    });

    // Wait for server to start so that the client can connect
    // this_thread::sleep_for(chrono::seconds(1));

    // Create a websocket server endpoint
    WsServer wsserver;

    try {
        wsserver.set_access_channels(websocketpp::log::alevel::all);
        wsserver.clear_access_channels(websocketpp::log::alevel::frame_payload);

        // Initialize Asio
        wsserver.init_asio();
        wsserver.set_http_handler(bind(&websocket_on_http, &wsserver, ::_1));

        // Register our message handler
        wsserver.set_message_handler(bind(&on_message,&wsserver,::_1,::_2));
        wsserver.set_validate_handler(bind(&validate, &wsserver, ::_1));

        // Listen on port 9002
        wsserver.listen(WS_PORT);

        // Start the server accept loop
        wsserver.start_accept();

        // Start the ASIO io_service run loop
        ChipLogError(NotSpecified, "WebSocket server started on port %d", WS_PORT);
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

    // server_thread.join();
    t.join(); // Wait for the child thread to end
    wsc.join();

    return 0;
}
