/*#include "../comm/Protocol.h"
#include <thread>
#include "../../include/sensor/HumiditySensor.h"
#include "../../include/utils/Random.h"

void runHumiditySensor() {
    HumiditySensor hum;

    while (true) {
        //float v = hum.read();
        float v = randomFloat(60.0f, 66.0f);
       buildHumidityMessage(v);
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    }
}*/
#include "../../include/sensor/HumiditySensor.h"
#include "../../include/utils/Random.h"

float HumiditySensor::read()
{
    return randomFloat(50.0f, 80.0f);
}