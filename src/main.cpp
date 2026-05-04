#include <iostream>
#include <thread>
#include <csignal>

#include "net/TcpServer.h"
#include "core/Globals.h"
#include "core/ThreadTasks.h"

int main()
{
    TcpServer server;
    g_tcpServer = &server;

    server.start(5555);

    std::signal(SIGINT, signalHandler);

    safePrint("Embedded Sensor Simulator v1.0");
    safePrint("Embedded Sensor Simulator Started");
    safePrint("--------------------------------");

    std::thread tempThread(runTemperatureSensor);
    std::thread pressureThread(runPressureSensor);
    std::thread humidityThread(runHumiditySensor);

    tempThread.join();
    pressureThread.join();
    humidityThread.join();

    safePrint("Simulator stopped.");

    return 0;
}