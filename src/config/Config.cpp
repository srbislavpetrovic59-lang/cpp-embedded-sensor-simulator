#include "config/Config.h"

#include <fstream>
#include <sstream>
#include <string>

static int extractInt(const std::string& content, const std::string& key, int defaultValue)
{
    size_t pos = content.find(key);
    if (pos == std::string::npos)
        return defaultValue;

    pos = content.find(":", pos);
    if (pos == std::string::npos)
        return defaultValue;

    std::stringstream ss(content.substr(pos + 1));
    int value;
    ss >> value;

    return ss.fail() ? defaultValue : value;
}

bool loadConfig(const std::string& filename, Config& config)
{
    std::ifstream file(filename);
    if (!file.is_open())
        return false;

    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string content = buffer.str();

    config.port = extractInt(content, "port", config.port);
    config.tempIntervalMs = extractInt(content, "temp_interval_ms", config.tempIntervalMs);
    config.pressureIntervalMs = extractInt(content, "pressure_interval_ms", config.pressureIntervalMs);
    config.humidityIntervalMs = extractInt(content, "humidity_interval_ms", config.humidityIntervalMs);

    return true;
}