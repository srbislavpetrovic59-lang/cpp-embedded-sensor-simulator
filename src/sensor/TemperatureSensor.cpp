#include "sensor/TemperatureSensor.h"
#include "utils/Random.h"

float TemperatureSensor::read()
{
    return randomFloat(24.0f, 26.0f);
}