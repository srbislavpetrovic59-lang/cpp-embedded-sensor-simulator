#include "utils/Random.h"
#include <random>

float randomFloat(float min, float max)
{
    static thread_local std::mt19937 generator(std::random_device{}());

    std::uniform_real_distribution<float> dist(min, max);

    return dist(generator);
}