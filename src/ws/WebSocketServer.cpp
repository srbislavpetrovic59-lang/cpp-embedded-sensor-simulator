#include "ws/WebSocketServer.h"

#include <iostream>

void WebSocketServer::start(int port)
{
    server.init_asio();

    server.set_open_handler(
        [this](websocketpp::connection_hdl hdl)
        {
            connections.insert(hdl);

            std::cout
                << "WebSocket client connected\n";
        });

    server.set_close_handler(
        [this](websocketpp::connection_hdl hdl)
        {
            connections.erase(hdl);

            std::cout
                << "WebSocket client disconnected\n";
        });

    server.listen(port);

    server.start_accept();

    std::cout
        << "WebSocket server listening on "
        << port
        << std::endl;

    server.run();
}

void WebSocketServer::broadcast(
    const std::string& message)
{
    for (auto& connection : connections)
    {
        server.send(
            connection,
            message,
            websocketpp::frame::opcode::text
        );
    }
}