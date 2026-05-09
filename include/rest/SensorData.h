#pragma once

struct SensorData
{
    float temperature = 0.0f;
    float pressure = 0.0f;
    float humidity = 0.0f;
};

extern SensorData g_sensorData;