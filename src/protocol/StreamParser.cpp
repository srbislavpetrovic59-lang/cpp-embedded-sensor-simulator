#include "protocol/StreamParser.h"

bool StreamParser::inputByte(uint8_t byte, ParsedPacket& out)
{
    buffer.push_back(byte);

    return tryParse(out);
}

bool StreamParser::tryParse(ParsedPacket& out)
{
    // Traži header
    while (!buffer.empty() && buffer[0] != 0xAA)
    {
        buffer.erase(buffer.begin());
    }

    if (buffer.size() < 3)
        return false;

    uint8_t len = buffer[2];

    size_t expectedSize = 3 + len + 1;

    if (buffer.size() < expectedSize)
        return false;

    std::vector<uint8_t> packet(
        buffer.begin(),
        buffer.begin() + expectedSize
    );

    if (parsePacket(packet, out))
    {
        buffer.erase(buffer.begin(), buffer.begin() + expectedSize);
        return true;
    }
    else
    {
        // loš paket → pomeri za 1 bajt
        buffer.erase(buffer.begin());
        return false;
    }
}

void StreamParser::reset()
{
    buffer.clear();
}