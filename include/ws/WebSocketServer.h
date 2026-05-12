#pragma once


#include <set>
#include <string>

#include <functional>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<
    websocketpp::config::asio
> WsServer;

class WebSocketServer
{
public:
    void start(int port);

    void broadcast(const std::string& message);

private:
    WsServer server;

    std::set<websocketpp::connection_hdl,
        std::owner_less<websocketpp::connection_hdl>>
        connections;
};