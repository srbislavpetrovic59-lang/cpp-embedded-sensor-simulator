#include "protocol/BinaryProtocol.h"

#include <cstring>   // memcpy

// jednostavan CRC: suma svih bajtova
static uint8_t calculateCRC(const std::vector<uint8_t>& data)
{
    uint8_t crc = 0;
    for (uint8_t b : data)
        crc += b;
    return crc;
}

std::vector<uint8_t> buildPacket(PacketType type, float value)
{
    std::vector<uint8_t> packet;

    // [HEADER]
    packet.push_back(0xAA);

    // [TYPE]
    packet.push_back(static_cast<uint8_t>(type));

    // [LEN] (float = 4 bytes)
    packet.push_back(4);

    // [DATA]
    uint8_t bytes[4];
    std::memcpy(bytes, &value, sizeof(float));

    for (int i = 0; i < 4; ++i)
        packet.push_back(bytes[i]);

    // [CRC]
    uint8_t crc = calculateCRC(packet);
    packet.push_back(crc);

    return packet;
}

bool parsePacket(const std::vector<uint8_t>& data, ParsedPacket& out)
{
    // Minimalna veličina: HEADER + TYPE + LEN + DATA(4) + CRC = 8
    if (data.size() < 8)
        return false;

    // HEADER
    if (data[0] != 0xAA)
        return false;

    // TYPE
    uint8_t rawType = data[1];

    switch (rawType)
    {
    case 0x01: out.type = PacketType::Temperature; break;
    case 0x02: out.type = PacketType::Pressure;    break;
    case 0x03: out.type = PacketType::Humidity;    break;
    default:   out.type = PacketType::Unknown;     break;
    }

    // LEN
    uint8_t len = data[2];
    if (len != 4)
        return false;

    // ukupna očekivana veličina
    if (data.size() != 3 + len + 1)
        return false;

    // CRC (poslednji bajt)
    uint8_t receivedCRC = data.back();

    std::vector<uint8_t> withoutCRC(data.begin(), data.end() - 1);
    uint8_t calculatedCRC = calculateCRC(withoutCRC);

    if (receivedCRC != calculatedCRC)
        return false;

    // DATA (float)
    float value;
    std::memcpy(&value, &data[3], sizeof(float));

    out.value = value;

    return true;
}