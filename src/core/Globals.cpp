#include "core/Globals.h"
#include "net/TcpServer.h"

#include <iostream>
#include <csignal>
#include <mutex>


// Define the global variable with type and storage specifier
std::atomic<bool> running{ true };

std::mutex coutMutex;
TcpServer* g_tcpServer = nullptr;

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