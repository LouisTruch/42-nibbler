#include "../../inc/Network/PacketManager.hpp"

PacketManager::PacketManager()
{
}

PacketManager::~PacketManager()
{
}

#include <stdexcept>    // std::runtime_error
#include <sys/socket.h> // send()
void PacketManager::sendPacket(int _fd, const Packet &packet) const
{
    if (send(_fd, packet.getDataToSend().c_str(), packet.getDataToSend().size() + 1, 0) == -1)
    {
        throw std::runtime_error("PacketManager::sendPacket: send() failed");
    }
}

void PacketManager::recvPacket(int _fd, Packet &packet) const
{
    char buffer[1024] = {0};
    int valread = read(_fd, buffer, 1024);
    if (valread == -1)
    {
        throw std::runtime_error("PacketManager::recvPacket: read() failed");
    }
    packet.getBufferRecv() = buffer;
}

const board_size_t PacketManager::getBoardSizeFromPacket(Packet &packet) const
{
    board_size_t boardSize;
    std::string data = packet.getBufferRecv();
    std::string delimiter = "\n";
    size_t pos = 0;
    std::string token;
    int i = 0;
    while ((pos = data.find(delimiter)) != std::string::npos)
    {
        token = data.substr(0, pos);
        if (i == 0)
        {
            if (token != Packet::_MANDATORY_HEADER[0])
                throw std::runtime_error("PacketManager::getBoardSizeFromPacket: Invalid packet");
        }
        else if (i == 1)
        {
            size_t posColon = token.find(":");
            if (posColon == std::string::npos)
                throw std::runtime_error("PacketManager::getBoardSizeFromPacket: Invalid packet");
            std::string lineBegin = token.substr(0, posColon);
            if (lineBegin != "width")
                throw std::runtime_error("PacketManager::getBoardSizeFromPacket: Invalid packet");
            token = token.substr(token.find(":") + 1);
            boardSize.x = std::stoi(token);
        }
        else if (i == 2)
        {
            size_t posColon = token.find(":");
            if (posColon == std::string::npos)
                throw std::runtime_error("PacketManager::getBoardSizeFromPacket: Invalid packet");
            std::string lineBegin = token.substr(0, posColon);
            if (lineBegin != "height")
                throw std::runtime_error("PacketManager::getBoardSizeFromPacket: Invalid packet");
            token = token.substr(token.find(":") + 1);
            boardSize.y = std::stoi(token);
        }
        else if (i > 2)
            throw std::runtime_error("PacketManager::getBoardSizeFromPacket: Invalid packet");
        data.erase(0, pos + delimiter.length());
        i++;
    }
    return boardSize;
}

const player_input_t PacketManager::getPlayerInputFromPacket(Packet &packet) const
{
    player_input_t playerInput;
    // std::string data = packet.getBufferRecv();
    // std::string delimiter = "\n";
    // size_t pos = 0;
    // std::string token;
    // int i = 0;
    // while ((pos = data.find(delimiter)) != std::string::npos)
    // {
    //     token = data.substr(0, pos);
    //     if (i == 0)
    //     {
    //         if (token != Packet::_MANDATORY_HEADER[0])
    //             throw std::runtime_error("PacketManager::getPlayerInputFromPacket: Invalid packet");
    //     }
    //     else if (i == 1)
    //     {
    //         size_t posColon = token.find(":");
    //         if (posColon == std::string::npos)
    //             throw std::runtime_error("PacketManager::getPlayerInputFromPacket: Invalid packet");
    //         std::string lineBegin = token.substr(0, posColon);
    //         if (lineBegin != "player_input")
    //             throw std::runtime_error("PacketManager::getPlayerInputFromPacket: Invalid packet");
    //         token = token.substr(token.find(":") + 1);
    //         playerInput = static_cast<player_input_t>(std::stoi(token));
    //     }
    //     else if (i > 1)
    //         throw std::runtime_error("PacketManager::getPlayerInputFromPacket: Invalid packet");
    //     data.erase(0, pos + delimiter.length());
    //     i++;
    // }
    return playerInput;
}
