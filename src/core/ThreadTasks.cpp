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
#include "config/Config.h"
#include "protocol/BinaryProtocol.h"
#include <iostream>
#include "database/TelemetryDatabase.h"
#include "ws/WebSocketServer.h"
#include "rest/SensorData.h"

void debugPrint(const std::vector<uint8_t>& data)
{
    for (auto b : data)
        std::cout << std::hex << std::setw(2)
        << std::setfill('0') << (int)b << " ";

    std::cout << std::dec << std::endl;
}

void runTemperatureSensor()
{
    TemperatureSensor sensor;

    while (running)
    {
        float value = sensor.read();
        g_database.insert("TEMP", value);
        g_sensorData.temperature = value;
        
        if (g_wsServer)
        {
                std::string json =
            "{"
            "\"temperature\":" + std::to_string(g_sensorData.temperature) + ","
            "\"pressure\":" + std::to_string(g_sensorData.pressure) + ","
            "\"humidity\":" + std::to_string(g_sensorData.humidity) +
            "}";

        g_wsServer->broadcast(json);
        }

        auto packet = buildPacket(PacketType::Temperature, value);

        if (g_tcpServer)
            g_tcpServer->sendBinary(packet);
        std::cout << "TEMP: " << value << std::endl;
        
        
        std::this_thread::sleep_for(
            std::chrono::milliseconds(g_config.tempIntervalMs));
        for (auto b : packet)
            printf("%02X ", b);
        printf("\n");
    }
}

void runPressureSensor() {
    PressureSensor sensor;

    while (running)
    {
        float value = sensor.read();
        g_database.insert("PRES", value);
        g_sensorData.pressure = value;
        
        if (g_wsServer)
        {
            std::string json =
                "{"
                "\"temperature\":" + std::to_string(g_sensorData.temperature) + ","
                "\"pressure\":" + std::to_string(g_sensorData.pressure) + ","
                "\"humidity\":" + std::to_string(g_sensorData.humidity) +
                "}";

            g_wsServer->broadcast(json);
        }
       
        auto packet = buildPacket(PacketType::Pressure, value);
        if (g_tcpServer)
            g_tcpServer->sendBinary(packet);
        std::cout << "PRES: " << value << std::endl;
        g_sensorData.pressure = value;

        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        for (auto b : packet)
            printf("%02X ", b);
        printf("\n");
    }
}
void runHumiditySensor() {
    HumiditySensor sensor;

    while (running)
    {
        float value = sensor.read();
        g_database.insert("HUM", value);
        g_sensorData.humidity = value;

       if (g_wsServer)
        {
            std::string json =
                "{"
                "\"temperature\":" + std::to_string(g_sensorData.temperature) + ","
                "\"pressure\":" + std::to_string(g_sensorData.pressure) + ","
                "\"humidity\":" + std::to_string(g_sensorData.humidity) +
                "}";

            g_wsServer->broadcast(json);
        }

        auto packet = buildPacket(PacketType::Humidity, value);
        if (g_tcpServer)
            g_tcpServer->sendBinary(packet);

        std::cout << "HUM:  " << value << std::endl;
        
        std::this_thread::sleep_for(std::chrono::seconds(2));
        for (auto b : packet)
            printf("%02X ", b);
        printf("\n");
    }
}

