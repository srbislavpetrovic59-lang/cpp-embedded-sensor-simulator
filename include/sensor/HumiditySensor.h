// HumiditySensor.h

#include "Sensor.h"

class HumiditySensor : public Sensor
{
public:
    HumiditySensor() = default;
    ~HumiditySensor() override = default;

    float read() override;
};