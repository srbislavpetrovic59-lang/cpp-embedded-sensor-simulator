#pragma once

#include <string>

// Build protocol messages
std::string buildTemperatureMessage(float value);
std::string buildPressureMessage(float value);
std::string buildHumidityMessage(float value);

// Send message (console for now)
void sendMessage(const std::string& msg);
