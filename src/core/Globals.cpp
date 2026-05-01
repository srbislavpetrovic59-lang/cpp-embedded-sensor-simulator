#include "core/Globals.h"

#include <iostream>
#include <csignal>
#include <mutex>

// thread-safe print
void safePrint(const std::string& msg)
{
    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout << msg << std::endl;
}
std::mutex coutMutex;

// Define the global variable with type and storage specifier
std::atomic<bool> running{ true };

// Signal handler function with explicit return type and parameter
void signalHandler(int signal)
{
    safePrint("Signal received: " + std::to_string(signal));
    running = false;
}