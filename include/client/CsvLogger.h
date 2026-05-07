#pragma once

#include <fstream>
#include <string>
#include "protocol/BinaryProtocol.h"

class CsvLogger
{
public:
    CsvLogger(const std::string& filename);
    void log(PacketType type, float value);

private:
    std::ofstream file;

    float temp = 0.0f;
    float pres = 0.0f;
    float hum = 0.0f;

    bool hasTemp = false;
    bool hasPres = false;
    bool hasHum = false;

    std::string getTimestamp();
};