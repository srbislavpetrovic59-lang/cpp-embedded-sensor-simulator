# Embedded Sensor Simulator
Current Release: v3.0
# Architecture
Multi-threaded embedded telemetry simulator written in modern C++.

The project simulates real-time sensor devices, streams binary telemetry data over TCP, supports multiple monitoring clients, logs telemetry to CSV, exposes a REST API, and provides live visualization using Python. 

## Features
# Embedded Sensor Simulation

- Temperature sensor
- Pressure sensor
- Humidity sensor
- Configurable update intervals

## TCP Networking

- TCP telemetry server
- Multi-client support
- Real-time binary streaming
- Concurrent client handling

## Binary Protocol

Custom lightweight binary protocol:

- Packet header
- Packet type
- Float payload
- Stream parser
- TCP-safe packet reconstruction
## Monitoring Client

Client application supports:

- Real-time packet decoding
- Live telemetry monitoring
- CSV logging
- Alarm system

## Alarm System

Threshold-based warnings:
- High temperature
- High pressure
- High humidity

## REST API
HTTP API for latest sensor values.
**Endpoints**
**Status**
GET /status
**Response:**
REST server running

**Latest Sensor Values**
GET /latest

**Example response:**
```text
{
  "temperature": 24.8,
  "pressure": 1013.2,
  "humidity": 45.1
}
```
---

## High Level Diagram
```text
                 +------------------+
                 |      main.cpp    |
                 | starts threads   |
                 +--------+---------+
                          |
      ---------------------------------------------
      |                    |                      |
      v                    v                      v

+-------------+     +-------------+      +-------------+
| Temp Thread |     | PressThread |      | Hum Thread  |
+------+------+     +------+------+      +------+------+
       |                   |                    |
       v                   v                    v

+-------------+     +-------------+      +-------------+
| TempSensor  |     | PressSensor |      | HumSensor   |
+------+------+     +------+------+      +------+------+
       |                   |                    |
       ------------------------------------------
                          |
                          v

                 +------------------+
                 |   Protocol.cpp   |
                 | build messages   |
                 +--------+---------+
                          |
                          v

                 +------------------+
                 |  sendMessage()   |
                 | Console output   |
                 +------------------+


```
## Project Structure
```text
EmbeddedSensorSimulator/
│
├── include/
│   ├── protocol/
│   ├── network/
│   ├── client/
│   └── rest/
│
├── src/
│   ├── protocol/
│   ├── network/
│   ├── sensor/
│   ├── client/
│   └── rest/
│
├── third_party/
│
├── config.json
├── CMakeLists.txt
├── README.md
└── .gitignore
```
## Technologies
- C++17
- WinSock2
- CMake
- Multi-threading
- TCP/IP
- REST API
- JSON
- Python
- matplotlib
## Build

**Requirements**
- Visual Studio 2022
- CMake
- Python 3.x

**Build Steps**
```text
git clone <repo_url>
cd EmbeddedSensorSimulator
```
Open project in Visual Studio and build using CMake.

## Running
**Start Server**

Run:
```text
   - ServerApp.exe
   - Start Client
```
## Start Client

**Run one or more:**
ClientApp.exe



## REST API
Open browser:

http://localhost:8080/status
http://localhost:8080/latest

## Python Live Graph
python plot.py
<img width="637" height="552" alt="image" src="https://github.com/user-attachments/assets/1d315187-eb50-4846-87f5-643f62c3b46e" />


## Example Console Output
- TEMP: 24.81
- PRES: 1013.2
- HUM: 45.3

## Future Improvements
```text
Web dashboard
SQLite telemetry storage
WebSocket streaming
Unreal Engine visualization
Authentication
Docker deployment 
```
## Author  

Srbislav Petrovic

## License

MIT License

