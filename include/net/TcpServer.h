#pragma once

#include <string>
#include <mutex>
#include <vector>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#endif

class TcpServer
{
public:
    TcpServer();
    ~TcpServer();

    bool start(int port);
    void stop();

    void acceptLoop();

    bool isClientConnected() const;

    void sendMessage(const std::string& message);
    
    void sendBinary(const std::vector<uint8_t>& data);

private:
    bool initializeSockets();
    void cleanupSockets();

    bool createListenSocket(int port);
    bool acceptClient();

    void closeClient();
    void closeServer();

private:
#ifdef _WIN32
    SOCKET listenSocket;
    SOCKET clientSocket;
#else
    int listenSocket;
    int clientSocket;
#endif

    bool running;
    bool clientConnected;

    mutable std::mutex socketMutex;
};