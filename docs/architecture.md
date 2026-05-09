# Architecture

## Overview
Multi-threaded embedded telemetry simulator written in modern C++.

The project simulates real-time sensor devices, streams binary telemetry data over TCP, supports multiple monitoring clients, logs telemetry to CSV, exposes a REST API, and provides live visualization using Python.

---

## Runtime Flow

When the application starts:

1. `main.cpp` initializes the system
2. CTRL+C signal handler is registered
3. Three worker threads are started:
   - Temperature thread
   - Pressure thread
   - Humidity thread
4. Each thread continuously:
   - reads sensor value
   - builds protocol message
   - sends output
   - waits configured interval

When CTRL+C is pressed:

1. `running = false`
2. all threads exit loop
3. application shuts down cleanly

---

## High Level Diagram

```text
Sensors
   ↓
TCP Server
   ↓
Binary Protocol
   ↓
REST API
   ↓
Web Dashboard
```
