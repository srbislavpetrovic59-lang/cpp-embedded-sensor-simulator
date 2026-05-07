#pragma once

#include <vector>
#include <cstdint>

enum class PacketType : uint8_t
{
    Temperature = 0x01,
    Pressure = 0x02,
    Humidity = 0x03,
    Unknown = 0xFF
};

std::vector<uint8_t> buildPacket(PacketType type, float value);

struct ParsedPacket
{
    PacketType type;
    float value;
};

bool parsePacket(const std::vector<uint8_t>& data, ParsedPacket& out);