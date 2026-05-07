#pragma once

#include <string>

struct Config
{
    int port = 5555;
    int tempIntervalMs = 1000;
    int pressureIntervalMs = 1500;
    int humidityIntervalMs = 2000;
};

bool loadConfig(const std::string& filename, Config& config);