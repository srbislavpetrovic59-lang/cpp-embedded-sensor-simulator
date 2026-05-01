#pragma once
#include "Sensor.h"

class PressureSensor : public Sensor
{
public:
    PressureSensor() = default;
    ~PressureSensor() override = default;

    float read() override;
};