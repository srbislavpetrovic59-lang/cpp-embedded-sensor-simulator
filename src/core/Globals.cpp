#include "core/Globals.h"
#include "net/TcpServer.h"

#include <iostream>
#include <csignal>
#include <mutex>
#include "config/Config.h"
#include "ws/WebSocketServer.h"


// Define the global variable with type and storage specifier
TelemetryDatabase g_database;
std::atomic<bool> running{ true };

std::mutex coutMutex;
TcpServer* g_tcpServer = nullptr;

Config g_config;
WebSocketServer* g_wsServer = nullptr;

// thread-safe print
void safePrint(const std::string& msg)
{
    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout << msg << std::endl;
}



// Signal handler function with explicit return type and parameter
void signalHandler(int signal)
{
    safePrint("Signal received: " + std::to_string(signal));
    running = false;
}