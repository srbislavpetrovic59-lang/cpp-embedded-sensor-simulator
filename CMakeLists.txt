cmake_minimum_required(VERSION 3.15)

project(EmbeddedSensorSimulator)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

# Server (bez client main)
file(GLOB_RECURSE SERVER_SOURCES
    src/*.cpp
)

list(FILTER SERVER_SOURCES EXCLUDE REGEX "src/client/.*")

add_executable(ServerApp ${SERVER_SOURCES} "include/client/CsvLogger.h" "src/client/CsvLogger.cpp" "src/client/AlarmSystem.cpp" "include/client/AlarmSystem.h" "include/rest/SensorData.h" "src/rest/SensorData.cpp" "include/rest/RestServer.h" "src/rest/RestServer.cpp")

# Client
add_executable(ClientApp    
    src/client/main.cpp
    src/protocol/BinaryProtocol.cpp
    src/protocol/StreamParser.cpp
 "include/client/CsvLogger.h" "src/client/CsvLogger.cpp" "src/client/AlarmSystem.cpp" "include/client/AlarmSystem.h" "include/rest/SensorData.h" "src/rest/SensorData.cpp" "include/rest/RestServer.h" "src/rest/RestServer.cpp")
