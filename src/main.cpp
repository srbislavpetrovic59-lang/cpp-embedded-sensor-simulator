#include <iostream>
#include <thread>
#include <csignal>

#include "core/Globals.h"
#include "core/ThreadTasks.h"

int main()
{
    std::signal(SIGINT, signalHandler);

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