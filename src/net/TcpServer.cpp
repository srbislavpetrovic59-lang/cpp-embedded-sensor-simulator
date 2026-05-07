#include "net/TcpServer.h"

#include <iostream>
#include <thread>

TcpServer::TcpServer()
    : listenSocket(INVALID_SOCKET),
    clientSocket(INVALID_SOCKET),
    running(false),
    clientConnected(false)
{
}

TcpServer::~TcpServer()
{
    stop();
}

bool TcpServer::start(int port)
{
    if (!initializeSockets())
        return false;

    if (!createListenSocket(port))
        return false;

    running = true;

    std::cout << "Listening on port " << port << std::endl;

    std::thread(&TcpServer::acceptLoop, this).detach();

    return true;
}

void TcpServer::stop()
{
    running = false;

    closeClient();
    closeServer();

    cleanupSockets();
}

bool TcpServer::isClientConnected() const
{
    return clientConnected;
}

void TcpServer::sendBinary(const std::vector<uint8_t>& data)
{
    std::lock_guard<std::mutex> lock(socketMutex);

    if (!clientConnected)
        return;

    int result = send(
        clientSocket,
        reinterpret_cast<const char*>(data.data()),
        static_cast<int>(data.size()),
        0
    );

    if (result == SOCKET_ERROR)
    {
        std::cout << "Client disconnected." << std::endl;
        closeClient();
    }
}

void TcpServer::sendMessage(const std::string& message)
{
    std::lock_guard<std::mutex> lock(socketMutex);

    if (!clientConnected)
        return;

    std::string data = message + "\r\n";

    int result = send(
        clientSocket,
        data.c_str(),
        static_cast<int>(data.size()),
        0
    );

    if (result == SOCKET_ERROR)
    {
        std::cout << "Client disconnected." << std::endl;
        closeClient();
    }
}

bool TcpServer::initializeSockets()
{
    WSADATA wsaData;

    return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
}

void TcpServer::cleanupSockets()
{
    WSACleanup();
}

bool TcpServer::createListenSocket(int port)
{
    listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (listenSocket == INVALID_SOCKET)
        return false;

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(
        listenSocket,
        reinterpret_cast<sockaddr*>(&serverAddr),
        sizeof(serverAddr)) == SOCKET_ERROR)
    {
        return false;
    }

    if (listen(listenSocket, 1) == SOCKET_ERROR)
        return false;

    return true;
}

void TcpServer::acceptLoop()
{
    while (running)
    {
        if (!clientConnected)
        {
            acceptClient();
        }

        std::this_thread::sleep_for(
            std::chrono::milliseconds(200));
    }
}

bool TcpServer::acceptClient()
{
    clientSocket = accept(listenSocket, nullptr, nullptr);

    if (clientSocket == INVALID_SOCKET)
        return false;

    clientConnected = true;

    std::cout << "Client connected." << std::endl;

    return true;
}

void TcpServer::closeClient()
{
    if (clientSocket != INVALID_SOCKET)
    {
        closesocket(clientSocket);
        clientSocket = INVALID_SOCKET;
    }

    clientConnected = false;
}

void TcpServer::closeServer()
{
    if (listenSocket != INVALID_SOCKET)
    {
        closesocket(listenSocket);
        listenSocket = INVALID_SOCKET;
    }
}