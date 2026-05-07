#include "client/CsvLogger.h"

#include <chrono>
#include <iomanip>
#include <sstream>

CsvLogger::CsvLogger(const std::string& filename)
{
    file.open(filename);

    file << "timestamp,temp,pressure,humidity\n";
}

void CsvLogger::log(PacketType type, float value)
{
    switch (type)
    {
    case PacketType::Temperature:
        temp = value;
        hasTemp = true;
        break;

    case PacketType::Pressure:
        pres = value;
        hasPres = true;
        break;

    case PacketType::Humidity:
        hum = value;
        hasHum = true;
        break;

    default:
        return;
    }

    // kada imamo sve → upiši red
    if (hasTemp && hasPres && hasHum)
    {
        file << getTimestamp() << ","
            << temp << ","
            << pres << ","
            << hum << "\n";

        file.flush();

        hasTemp = hasPres = hasHum = false;
    }
}

std::string CsvLogger::getTimestamp()
{
    using namespace std::chrono;

    auto now = system_clock::now();
    auto time = system_clock::to_time_t(now);

    auto ms = duration_cast<milliseconds>(
        now.time_since_epoch()) % 1000;

    std::tm tm;
    localtime_s(&tm, &time);

    std::ostringstream oss;

    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S")
        << "." << std::setfill('0') << std::setw(3) << ms.count();

    return oss.str();
}