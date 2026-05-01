#pragma once

#include <string>
#include <mutex>
#include <atomic>

// shared variables
extern std::mutex coutMutex;
extern std::atomic<bool> running;

// shared functions
void safePrint(const std::string& msg);
void signalHandler(int signal);