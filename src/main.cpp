#include <iostream>
#include <thread>

#include "core/Globals.h"
#include "net/TcpServer.h"
#include "core/ThreadTasks.h"
#include "config/Config.h"
#include "protocol/StreamParser.h"
#include "protocol/BinaryProtocol.h"
#include "ws/WebSocketServer.h"
#include "rest/RestServer.h"

#include "database/TelemetryDatabase.h"


void testStreamParser()
{
    StreamParser parser;
    ParsedPacket parsed;

    // napravi jedan paket
    auto packet = buildPacket(PacketType::Temperature, 25.5f);

    // simuliraj byte-by-byte prijem
    for (uint8_t b : packet)
    {
        if (parser.inputByte(b, parsed))
        {
            std::cout << "Parsed: " << parsed.value << std::endl;
        }
    }
}



int main()
{
    safePrint("Embedded Sensor Simulator v2.0");

    // učitaj config
    if (!loadConfig("config.json", g_config))
    {
        safePrint("Config not found, using defaults");
    }
    if (!g_database.open("telemetry.db"))
    {
        safePrint("Failed to open database");
        return -1;
    }

     testStreamParser();
    // TCP server
    TcpServer server;
    g_tcpServer = &server;

    if (!server.start(g_config.port))
    {
        safePrint("Failed to start TCP server");
        return -1;
    }
    RestServer restServer;

    std::thread restThread([&]()
        {
            restServer.start(8080, g_database);
        });
    WebSocketServer wsServer;
    g_wsServer = &wsServer;

    std::thread wsThread([&wsServer]()
        {
            wsServer.start(8090);
        });

    // pokreni threadove senzora
 /*   std::thread t1(runTemperatureSensor);
    std::thread t2(runPressureSensor);
    std::thread t3(runHumiditySensor);
    */
    // čekaj exit (Ctrl+C ili ENTER)
    std::cin.get();

    running = false;

   /* t1.join();
    t2.join();
    t3.join();*/

    server.stop();
    wsThread.detach();
    restThread.detach();

    safePrint("Shutting down...");

    return 0;
}