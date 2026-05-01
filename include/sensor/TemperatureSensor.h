// TemperatureSensor.h

#include "Sensor.h"

class TemperatureSensor : public Sensor
{
public:
    TemperatureSensor() = default;
    ~TemperatureSensor() override = default;

    float read() override;
};