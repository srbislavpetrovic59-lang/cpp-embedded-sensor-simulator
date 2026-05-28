#pragma once

#include <string>
#include <mutex>
#include <vector>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
using SOCKET = int;
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
#endif

class TcpServer
{
public:
    TcpServer();
    ~TcpServer();

    bool start(int port);
    void stop();

   

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

    void acceptLoop();
private:
#ifdef _WIN32
    SOCKET listenSocket;
    SOCKET clientSocket;
#else
    int listenSocket;
    int clientSocket;
#endif
    SOCKET serverSocket = INVALID_SOCKET;

    std::vector<SOCKET> clients;

    std::mutex clientMutex;

    bool running;
    bool clientConnected;

    mutable std::mutex socketMutex;
};