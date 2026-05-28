#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>

std::string getTimestamp()
{
    using namespace std::chrono;

    auto now = system_clock::now();

    auto ms =
        duration_cast<milliseconds>(
            now.time_since_epoch()) % 1000;

    std::time_t timeNow =
        system_clock::to_time_t(now);

    std::tm localTime{};
    #ifdef _WIN32
        localtime_s(&localTime, &timeNow);
    #else
        localtime_r(&timeNow, &localTime);
    #endif

    std::ostringstream oss;

    oss << "["
        << std::setfill('0')
        << std::setw(2) << localTime.tm_hour << ":"
        << std::setw(2) << localTime.tm_min << ":"
        << std::setw(2) << localTime.tm_sec << "."
        << std::setw(3) << ms.count()
        << "] ";

    return oss.str();
}