# Architecture

## Overview

Embedded Sensor Simulator is a C++ console application that simulates a real embedded device sending live telemetry data from multiple sensors.

The project is structured in independent modules:

- **Sensors** → generate realistic values
- **ThreadTasks** → run each sensor in its own thread
- **Protocol** → formats messages
- **Globals** → shared state (mutex, running flag)
- **Output Layer** → console now, UART/TCP later

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
