#pragma once

#include <vector>
#include <cstdint>
#include "protocol/BinaryProtocol.h"

class StreamParser
{
public:
    bool inputByte(uint8_t byte, ParsedPacket& out);

private:
    std::vector<uint8_t> buffer;

    bool tryParse(ParsedPacket& out);
    void reset();
};