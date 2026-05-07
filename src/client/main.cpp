#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#include "protocol/StreamParser.h"
#include "client/CsvLogger.h"
#include "client/AlarmSystem.h"

void printPacket(const ParsedPacket& packet)
{
    switch (packet.type)
    {
    case PacketType::Temperature:
        std::cout << "TEMP: " << packet.value << std::endl;
        break;

    case PacketType::Pressure:
        std::cout << "PRES: " << packet.value << std::endl;
        break;

    case PacketType::Humidity:
        std::cout << "HUM:  " << packet.value << std::endl;
        break;

    default:
        std::cout << "UNKNOWN: " << packet.value << std::endl;
        break;
    }
}

int main()
{
    const char* ip = "127.0.0.1";
    const int port = 5555;

    WSADATA wsa;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        std::cout << "WSAStartup failed\n";
        return -1;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (sock == INVALID_SOCKET)
    {
        std::cout << "Socket creation failed\n";
        WSACleanup();
        return -1;
    }

    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if (inet_pton(AF_INET, ip, &server.sin_addr) <= 0)
    {
        std::cout << "Invalid address\n";

        closesocket(sock);
        WSACleanup();

        return -1;
    }

    std::cout << "Connecting to server...\n";

    if (connect(sock, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
    {
        std::cout << "Connection failed\n";

        closesocket(sock);
        WSACleanup();

        return -1;
    }

    std::cout << "Connected!\n";

    StreamParser parser;
    ParsedPacket parsed;

    CsvLogger logger("data.csv");

    AlarmSystem alarm;

    char buffer[512];

    while (true)
    {
        int received = recv(sock, buffer, sizeof(buffer), 0);

        if (received == 0)
        {
            std::cout << "Server disconnected\n";
            break;
        }

        if (received < 0)
        {
            std::cout << "Receive error\n";
            break;
        }

        for (int i = 0; i < received; ++i)
        {
            uint8_t dataByte =
                static_cast<uint8_t>(buffer[i]);

            if (parser.inputByte(dataByte, parsed))
            {
                printPacket(parsed);

                logger.log(parsed.type, parsed.value);

                switch (parsed.type)
                {
                case PacketType::Temperature:
                    alarm.checkTemperature(parsed.value);
                    break;

                case PacketType::Pressure:
                    alarm.checkPressure(parsed.value);
                    break;

                case PacketType::Humidity:
                    alarm.checkHumidity(parsed.value);
                    break;

                default:
                    break;
                }
            }
        }
    }

    closesocket(sock);

    WSACleanup();

    return 0;
}