#include "sensor/PressureSensor.h"
#include "utils/Random.h"


float PressureSensor::read()
{
    return randomFloat(100.8f, 101.8f);
}