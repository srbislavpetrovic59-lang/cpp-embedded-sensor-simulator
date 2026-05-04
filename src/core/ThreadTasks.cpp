#include <thread>
#include <chrono>
#include <sstream>
#include <iomanip>
#include "core/ThreadTasks.h"
#include "core/Globals.h"
#include "sensor/TemperatureSensor.h"
#include "sensor/PressureSensor.h"
#include "sensor/HumiditySensor.h"
#include "net/TcpServer.h"
#include "comm/Protocol.h"

void runTemperatureSensor() {
    TemperatureSensor sensor;

    while (running)
    {
        float value = sensor.read();

        std::ostringstream msg;
        msg << "<TEMP:"
            << std::fixed
            << std::setprecision(2)
            << value
            << ">";

        sendMessage(msg.str());

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
void runPressureSensor() {
    PressureSensor sensor;

    while (running)
    {
        float value = sensor.read();

        std::ostringstream msg;
        msg << "<PRES:"
            << std::fixed
            << std::setprecision(2)
            << value
            << ">";

        sendMessage(msg.str());

        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    }
}
void runHumiditySensor() {
    HumiditySensor sensor;

    while (running)
    {
        float value = sensor.read();

        std::ostringstream msg;
        msg << "<HUM:"
            << std::fixed
            << std::setprecision(2)
            << value
            << ">";

        sendMessage(msg.str());

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}