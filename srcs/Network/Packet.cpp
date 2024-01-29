#include "../../inc/Network/Packet.hpp"
#include <string>

// Used to receive data
Packet::Packet(std::string::size_type size)
{
    _bufferRecv.resize(size);
}

Packet::Packet(board_size_t boardSize)
{
    // Format data into packet
    _dataToSend = "board_size\n";
    _dataToSend += "width:" + std::to_string(boardSize.x) + "\n";
    _dataToSend += "height:" + std::to_string(boardSize.y) + "\n";
}

Packet::Packet(std::string_view data)
{
    _dataToSend = data;
}

Packet::~Packet()
{
}

Packet::Packet(const Packet &other)
{
    *this = other;
}

Packet &Packet::operator=(const Packet &other)
{
    if (this != &other)
    {
        _bufferRecv = other._bufferRecv;
        _dataToSend = other._dataToSend;
    }
    return *this;
}

const std::string &Packet::getDataToSend() const noexcept
{
    return _dataToSend;
}

std::string &Packet::getBufferRecv() noexcept
{
    return _bufferRecv;
}