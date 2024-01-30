#include "../../inc/Network/Packet.hpp"
#include "../../inc/Log/Logger.hpp"
#include <string>

// Used to receive data
Packet::Packet(std::string::size_type size)
{
    _bufferRecv.clear();
    _bufferRecv.resize(size);
}

Packet::Packet(const board_size_t &boardSize)
{
    // Format data into packet
    _dataToSend = _HEADERS_BOARDSIZE[0];
    _dataToSend += "\n";
    _dataToSend += _HEADERS_BOARDSIZE[1];
    _dataToSend += std::to_string(boardSize.x) + "\n";
    _dataToSend += _HEADERS_BOARDSIZE[2];
    _dataToSend += std::to_string(boardSize.y) + "\n";
}

Packet::Packet(const GameData_t &gameData)
{
    _dataToSend = _HEADERS_GAMEDATA[0];
    _dataToSend += "\n";
    _dataToSend += _HEADERS_GAMEDATA[1];
    _dataToSend += "d";
    if (gameData.p0.dir > 0)
        _dataToSend += "+";
    _dataToSend += std::to_string(gameData.p0.dir) + ";";
    for (const auto &point : gameData.p0.body)
    {
        _dataToSend += std::to_string(point.x) + ":" + std::to_string(point.y) + ";";
    }
    _dataToSend += "\n";
    _dataToSend += _HEADERS_GAMEDATA[2];
    _dataToSend += "d";
    if (gameData.p1->dir > 0)
        _dataToSend += "+";
    _dataToSend += std::to_string(gameData.p1->dir) + ";";
    for (const auto &point : gameData.p1->body)
    {
        _dataToSend += std::to_string(point.x) + ":" + std::to_string(point.y) + ";";
    }
    _dataToSend += "\n";
    _dataToSend += _HEADERS_GAMEDATA[3];
    _dataToSend += std::to_string(gameData.food.x) + ":" + std::to_string(gameData.food.y) + "\n";
}

Packet::Packet(const player_input_t &playerInput)
{
    _dataToSend = _HEADERS_INPUT[0];
    _dataToSend += "\n";
    _dataToSend += _HEADERS_INPUT[1];
    if (playerInput > 0)
        _dataToSend += "+";
    _dataToSend += std::to_string(playerInput) + "\n";
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