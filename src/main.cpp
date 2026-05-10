#include <iostream>
#include <thread>

#include "core/Globals.h"
#include "net/TcpServer.h"
#include "core/ThreadTasks.h"
#include "config/Config.h"
#include "protocol/StreamParser.h"
#include "protocol/BinaryProtocol.h"

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
    testStreamParser();
    // TCP server
    TcpServer server;
    g_tcpServer = &server;

    if (!server.start(g_config.port))
    {
        safePrint("Failed to start TCP server");
        return -1;
    }
	

    // pokreni threadove senzora
    std::thread t1(runTemperatureSensor);
    std::thread t2(runPressureSensor);
    std::thread t3(runHumiditySensor);

    // čekaj exit (Ctrl+C ili ENTER)
    std::cin.get();

    running = false;

    t1.join();
    t2.join();
    t3.join();

    server.stop();

    safePrint("Shutting down...");

    return 0;
}