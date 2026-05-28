#include "net/TcpServer.h"

#include <iostream>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

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
    #ifdef _WIN32
    WSADATA wsa;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        return false;
    }
    #endif
    serverSocket =
        socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (serverSocket == INVALID_SOCKET)
        return false;

    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket,
        (sockaddr*)&server,
        sizeof(server)) == SOCKET_ERROR)
    {
        return false;
    }

    listen(serverSocket, SOMAXCONN);

    std::cout << "Listening on port "
        << port << std::endl;

    std::thread(
        &TcpServer::acceptLoop,
        this
    ).detach();

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
    #ifdef _WIN32
        WSADATA wsaData;
        return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
    #else
        return true;
    #endif
}

void TcpServer::cleanupSockets()
{
    #ifdef _WIN32
        WSACleanup();
     #endif   
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
    while (true)
    {
        SOCKET client =
            accept(serverSocket, nullptr, nullptr);

        if (client != INVALID_SOCKET)
        {
            std::lock_guard<std::mutex>
                lock(clientMutex);

            clients.push_back(client);

            std::cout
                << "Client connected. Total: "
                << clients.size()
                << std::endl;
        }
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
        #ifdef _WIN32
        closesocket(clientSocket);
        #else
        close(clientSocket);
        #endif
        clientSocket = INVALID_SOCKET;
    }

    clientConnected = false;
}

void TcpServer::closeServer()
{
    if (listenSocket != INVALID_SOCKET)
    {
        #ifdef _WIN32
        closesocket(listenSocket);
        #else
        close(listenSocket);
        #endif
        listenSocket = INVALID_SOCKET;
    }
}

void TcpServer::sendBinary(
    const std::vector<uint8_t>& data)
{
    std::lock_guard<std::mutex>
        lock(clientMutex);

    for (auto it = clients.begin();
        it != clients.end();)
    {
        int result = send(
            *it,
            reinterpret_cast<const char*>(data.data()),
            static_cast<int>(data.size()),
            0
        );

        if (result == SOCKET_ERROR)
        {
            #ifdef _WIN32
            closesocket(*it);
            #else
            close(*it);
            #endif

            it = clients.erase(it);

            std::cout
                << "Client disconnected. Total: "
                << clients.size()
                << std::endl;
        }
        else
        {
            ++it;
        }
    }
}