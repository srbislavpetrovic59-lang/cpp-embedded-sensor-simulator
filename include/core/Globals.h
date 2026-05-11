#pragma once

#include <string>
#include <mutex>
#include <atomic>
#include "config/Config.h"
#include "database/TelemetryDatabase.h"

extern TelemetryDatabase g_database;
class TcpServer;

// shared variables
extern std::mutex coutMutex;
extern std::atomic<bool> running;
extern TcpServer* g_tcpServer;
extern Config g_config;

// shared functions
void safePrint(const std::string& msg);
void signalHandler(int signal);