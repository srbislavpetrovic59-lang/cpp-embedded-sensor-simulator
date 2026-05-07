#include "sensor/HumiditySensor.h"
#include "utils/Random.h"

float HumiditySensor::read()
{
    return randomFloat(45.0f, 80.0f);
}