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

#pragma once

#include "client.hpp"
#include "config/asio_no_tls_client.hpp"
#include <json/json.h>
#include <iostream>

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

typedef websocketpp::client<websocketpp::config::asio_client> WsClient;

static std::queue<Json::Value> reportQueue;
static std::queue<Json::Value> subscribeReportQueue;
static std::mutex reportQueueMutex;
static std::mutex subscribeReportQueueMutex;

class WebSocketClient {
public:
    WebSocketClient() {

        // Set logging to be pretty verbose (everything except message payloads)
        m_client.set_access_channels(websocketpp::log::alevel::all);
        m_client.clear_access_channels(websocketpp::log::alevel::frame_payload);

        // Initialize ASIO
        m_client.init_asio();

        // Register event handlers
        m_client.set_open_handler(bind(&WebSocketClient::onOpen,this,::_1));
        m_client.set_close_handler(bind(&WebSocketClient::onClose,this,::_1));
        m_client.set_fail_handler(bind(&WebSocketClient::onFail,this,::_1));
        m_client.set_message_handler(bind(&WebSocketClient::onMessage,this,::_1,::_2));
        //m_client.set_pong_handler(bind(&WebSocketClient::onPongMessage,this,::_1));
        //m_client.set_ping_handler(bind(&WebSocketClient::onPing,this,::_1))

        // Start ASIO io_service run loop
        m_client.start_perpetual();
        //m_client.run();
        m_thread = websocketpp::lib::make_shared<websocketpp::lib::thread>(&WsClient::run, &m_client);
    }

    ~WebSocketClient() {
        m_client.stop_perpetual();
        m_thread->join();
    }

    void connect(const std::string& ws_url){
        websocketpp::lib::error_code ec;
        m_connection_ptr = m_client.get_connection(ws_url, ec);
        if (ec) {
            std::cout << "could not create connection because: " << ec.message() << std::endl;
        }
        m_client.connect(m_connection_ptr);
    }

    std::string connection_staus() {
        //websocketpp::session::state state = m_connection_ptr->get_state();
        std::string status;
        if(m_connection_ptr->get_state() == websocketpp::session::state::open) {
            status = "Open";
        }
        else if (m_connection_ptr->get_state() == websocketpp::session::state::connecting)
        {
            status = "Connecting";
        }
        else if (m_connection_ptr->get_state() == websocketpp::session::state::closing)
        {
            status = "Closing";
        }
        else if (m_connection_ptr->get_state() == websocketpp::session::state::closed)
        {
            status = "Closed";
        }
        else
        {
            status = "Unknown";
        }
        return status;
    }

    void sendMessage(const std::string& msg) {
        m_client.send(m_hdl, msg, websocketpp::frame::opcode::text);
    }

    void sendPing(){
        m_client.ping(m_hdl, "");
    }

    void enqueueReport(const Json::Value& report) {
        std::lock_guard<std::mutex> lock(reportQueueMutex);
        reportQueue.push(report);
    }

    void enqueueSubscribeReport(const Json::Value& report) {
        std::lock_guard<std::mutex> lock(subscribeReportQueueMutex);
        subscribeReportQueue.push(report);
    }

    Json::Value dequeueReport() {
        std::lock_guard<std::mutex> lock(reportQueueMutex);
        Json::Value report = reportQueue.front();
        reportQueue.pop();
        return report;
    }

    Json::Value dequeueSubscribeReport() {
        std::lock_guard<std::mutex> lock(subscribeReportQueueMutex);
        Json::Value report = subscribeReportQueue.front();
        subscribeReportQueue.pop();
        return report;
    }

private:
    WsClient m_client;
    websocketpp::connection_hdl m_hdl;
    websocketpp::lib::shared_ptr<websocketpp::lib::thread> m_thread;
    std::string m_recieve_message;
    WsClient::connection_ptr m_connection_ptr;

    // Event handlers
    void onOpen(websocketpp::connection_hdl hdl) {
        m_hdl = hdl;
        std::cout << "Connected!" << std::endl;
    }

    void onClose(websocketpp::connection_hdl hdl) {
        std::cout << "Disconnected!" << std::endl;
    }

    void onFail(websocketpp::connection_hdl hdl) {
        std::cout << "Connection failed!" << std::endl;
    }

    void onMessage(websocketpp::connection_hdl hdl, websocketpp::config::asio_client::message_type::ptr msg) {
        m_recieve_message = msg->get_payload();
        Json::Reader reader;
        Json::Value recieveMessage, resultsReport;
        if (reader.parse(m_recieve_message, recieveMessage))
        {
            if (recieveMessage.isMember("subscribe_results"))
            {
                resultsReport = recieveMessage["subscribe_results"];
                enqueueSubscribeReport(resultsReport);
            }
            else
            {
                resultsReport = recieveMessage["results"];
                enqueueReport(resultsReport);
            }
        }
    }
};