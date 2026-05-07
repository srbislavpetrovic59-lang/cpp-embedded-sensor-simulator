#include "client/AlarmSystem.h"

#include <iostream>

void AlarmSystem::checkTemperature(float value)
{
    if (value > 30.0f)
    {
        std::cout << "WARNING: HIGH TEMPERATURE: "
            << value << std::endl;
    }
}

void AlarmSystem::checkPressure(float value)
{
    if (value > 1020.0f)
    {
        std::cout << "WARNING: HIGH PRESSURE: "
            << value << std::endl;
    }
}

void AlarmSystem::checkHumidity(float value)
{
    if (value > 70.0f)
    {
        std::cout << "WARNING: HIGH HUMIDITY: "
            << value << std::endl;
    }
}