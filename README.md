Embedded Sensor Simulator
C++ console application that simulates an embedded device sending live sensor telemetry.

Features
Multithreaded architecture
Temperature / Pressure / Humidity sensors
Randomized realistic values
Thread-safe console output
Cross-platform (Windows / Linux)
Ready for UART / TCP integration
Example Output
TEMP:24.83 PRES:101.22 HUM:48.10

Technologies
C++17
std::thread
mutex
chrono
Build
mkdir build
cd build
cmake ..
make

Run
./EmbeddedSimulator

Future Improvements
TCP socket telemetry
Config file
CRC protocol
Logging to file

Author

Srbislav Petrovic

