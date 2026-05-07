#include "sensor/PressureSensor.h"
#include "utils/Random.h"


float PressureSensor::read()
{
    return randomFloat(100.8f, 103.8f);
}