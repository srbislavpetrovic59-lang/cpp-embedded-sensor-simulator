cmake_minimum_required(VERSION 3.15)

project(EmbeddedSensorSimulator)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_compile_definitions(
    ASIO_STANDALONE
    _WEBSOCKETPP_CPP11_STL_
 
)

# Client
add_executable(ClientApp    
    src/client/main.cpp
    src/protocol/BinaryProtocol.cpp
    src/protocol/StreamParser.cpp
    src/database/TelemetryDatabase.cpp
 "include/client/CsvLogger.h" "src/client/CsvLogger.cpp" "src/client/AlarmSystem.cpp" "include/client/AlarmSystem.h" "include/rest/SensorData.h" "src/rest/SensorData.cpp"
 "include/rest/RestServer.h" "src/rest/RestServer.cpp"  "third_party/sqlite/sqlite3.c" "third_party/sqlite/sqlite3.h" )


target_include_directories(ClientApp PRIVATE
    third_party

)


# Server (bez client main)
# Server
file(GLOB_RECURSE SERVER_SOURCES
    src/*.cpp
)

list(FILTER SERVER_SOURCES EXCLUDE REGEX "src/client/.*")

add_executable(ServerApp
    ${SERVER_SOURCES}
    third_party/sqlite/sqlite3.c
)

target_include_directories(ServerApp PRIVATE
    include
    third_party
    third_party/websocketpp
    third_party/asio/include
)