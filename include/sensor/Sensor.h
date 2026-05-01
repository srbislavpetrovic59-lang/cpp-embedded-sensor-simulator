// Sensor.h
#pragma once

class Sensor {
public:
    virtual float read() = 0;
    virtual ~Sensor() = default;
};