// Protocol.cpp
#include <iostream>
#include <iomanip>
#include <mutex>
#include "core/Globals.h"

// ----------------------------------
// Build TEMP message
// ----------------------------------
void static buildTemperatureMessage(float value) {
    std::cout << "<TEMP:" << std::fixed << std::setprecision(2)
        << value << ">" << std::endl;
}

// ----------------------------------
// Build TEMP message
// ----------------------------------
void static buildPressureMessage(float value) {
    std::cout << "<PRESSURE:" << std::fixed << std::setprecision(2)
        << value << ">" << std::endl;
}

// ----------------------------------
// Build PRES message
// ----------------------------------
void static buildHumidityMessage(float value) {
    std::cout << "<HUMIDITY:" << std::fixed << std::setprecision(2)
        << value << ">" << std::endl;
}

// ----------------------------------
// Thread-safe print function
// ----------------------------------
static std::mutex printMutex;


// ----------------------------------
// Send message
// ----------------------------------
void sendMessage(const std::string& msg)
{
    safePrint(msg);
}