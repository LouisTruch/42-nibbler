#include "../../inc/Network/PacketManager.hpp"
#include "../../inc/Log/Logger.hpp"

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
    // LOG_DEBUG("Packet sent:\n" + packet.getDataToSend());
}

void PacketManager::recvPacket(int _fd, Packet &packet) const
{
    int byteRead = recv(_fd, &packet.getBufferRecv()[0], packet.getBufferRecv().size(), 0);
    if (byteRead == -1)
        throw std::runtime_error("PacketManager::recvPacket: read() failed");
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
        // std::cout << "token = " << token << std::endl;
        if (i == 0)
        {
            if (token != Packet::_HEADERS_BOARDSIZE[i])
                throw std::runtime_error("PacketManager::getBoardSizeFromPacket: Invalid packet");
        }
        else if (i == 1)
        {
            size_t posColon = token.find(":");
            if (posColon == std::string::npos)
                throw std::runtime_error("PacketManager::getBoardSizeFromPacket: Invalid packet");
            std::string lineBegin = token.substr(0, posColon + 1);
            if (lineBegin != Packet::_HEADERS_BOARDSIZE[i])
                throw std::runtime_error("PacketManager::getBoardSizeFromPacket: Invalid packet");
            token = token.substr(token.find(":") + 1);
            boardSize.x = std::stoi(token);
        }
        else if (i == 2)
        {
            size_t posColon = token.find(":");
            if (posColon == std::string::npos)
                throw std::runtime_error("PacketManager::getBoardSizeFromPacket: Invalid packet");
            std::string lineBegin = token.substr(0, posColon + 1);
            if (lineBegin != Packet::_HEADERS_BOARDSIZE[i])
                throw std::runtime_error("PacketManager::getBoardSizeFromPacket: Invalid packet");
            token = token.substr(token.find(":") + 1);
            boardSize.y = std::stoi(token);
            return boardSize;
        }
        else if (i > 2)
            throw std::runtime_error("PacketManager::getBoardSizeFromPacket: Invalid packet");
        data.erase(0, pos + delimiter.length());
        i++;
    }
    return boardSize;
}

const GameData_t PacketManager::getGameDataFromPacket(Packet &packet) const
{
    GameData_t gameData;
    std::string &data = packet.getBufferRecv();
    // LOG_DEBUG("PacketManager::getGameDataFromPacket: data =\n" + data);
    std::string delimiter = "\n";
    size_t pos = 0;
    std::string token;
    size_t i = 0;
    while ((pos = data.find(delimiter)) != std::string::npos)
    {
        token = data.substr(0, pos);
        if (i == 0)
        {
            if (token != Packet::_HEADERS_GAMEDATA[i])
                throw std::runtime_error("PacketManager::getGameDataFromPacket: Invalid packet");
        }
        else if (i == 1)
        {
            size_t posColon = token.find(":");
            if (posColon == std::string::npos)
                throw std::runtime_error("PacketManager::getGameDataFromPacket: Invalid packet");
            std::string lineBegin = token.substr(0, posColon + 1);
            if (lineBegin != Packet::_HEADERS_GAMEDATA[i])
                throw std::runtime_error("PacketManager::getGameDataFromPacket: Invalid packet");
            token = token.substr(token.find(":") + 1);
            gameData.p0.idx = 0;
            std::string delimiter2 = ";";
            size_t pos2 = 0;
            std::string token2;
            size_t j = 0;
            while ((pos2 = token.find(delimiter2)) != std::string::npos)
            {
                token2 = token.substr(0, pos2);
                if (j == 0)
                {
                    if (token2[0] == 'd')
                    {
                        gameData.p0.dir = static_cast<player_direction>(std::stoi(token2.substr(1)));
                    }
                    else
                    {
                        throw std::runtime_error("PacketManager::getGameDataFromPacket: Invalid packet");
                    }
                }
                else if (j > 0)
                {
                    point_t point;
                    point.x = std::stoi(token2.substr(0, token2.find(":")));
                    point.y = std::stoi(token2.substr(token2.find(":") + 1));
                    gameData.p0.body.push_back(point);
                }
                if (j > 1000)
                    throw std::runtime_error("PacketManager::getGameDataFromPacket: Invalid packet");
                token.erase(0, pos2 + delimiter2.length());
                j++;
            }
        }
        else if (i == 2)
        {
            size_t posColon = token.find(":");
            if (posColon == std::string::npos)
                throw std::runtime_error("PacketManager::getGameDataFromPacket: Invalid packet");
            std::string lineBegin = token.substr(0, posColon + 1);
            if (lineBegin != Packet::_HEADERS_GAMEDATA[i])
                throw std::runtime_error("PacketManager::getGameDataFromPacket: Invalid packet");
            gameData.p1 = std::make_optional(Snake_t());
            gameData.p1->idx = 1;
            token = token.substr(token.find(":") + 1);
            std::string delimiter2 = ";";
            size_t pos2 = 0;
            std::string token2;
            size_t j = 0;
            while ((pos2 = token.find(delimiter2)) != std::string::npos)
            {
                token2 = token.substr(0, pos2);
                if (j == 0)
                {
                    if (token2[0] == 'd')
                    {
                        gameData.p1->dir = static_cast<player_direction>(std::stoi(token2.substr(1)));
                    }
                    else
                    {
                        throw std::runtime_error("PacketManager::getGameDataFromPacket: Invalid packet");
                    }
                }
                else if (j > 0)
                {
                    point_t point;
                    point.x = std::stoi(token2.substr(0, token2.find(":")));
                    point.y = std::stoi(token2.substr(token2.find(":") + 1));
                    gameData.p1->body.push_back(point);
                }
                if (j > 1000)
                    throw std::runtime_error("PacketManager::getGameDataFromPacket: Invalid packet");
                token.erase(0, pos2 + delimiter2.length());
                j++;
            }
        }
        else if (i == 3)
        {
            size_t posColon = token.find(":");
            if (posColon == std::string::npos)
                throw std::runtime_error("PacketManager::getGameDataFromPacket: Invalid packet");
            std::string lineBegin = token.substr(0, posColon + 1);
            if (lineBegin != Packet::_HEADERS_GAMEDATA[i])
                throw std::runtime_error("PacketManager::getGameDataFromPacket: Invalid packet");
            token = token.substr(token.find(":") + 1);
            gameData.food.x = std::stoi(token.substr(0, token.find(":")));
            gameData.food.y = std::stoi(token.substr(token.find(":") + 1));
        }
        else if (i == 4)
        {
            size_t posColon = token.find(":");
            if (posColon == std::string::npos)
                throw std::runtime_error("PacketManager::getGameDataFromPacket: Invalid packet");
            std::string lineBegin = token.substr(0, posColon + 1);
            if (lineBegin != Packet::_HEADERS_GAMEDATA[i])
                throw std::runtime_error("PacketManager::getGameDataFromPacket: Invalid packet");
            token = token.substr(token.find(":") + 1);
            gameData.playEatingSound = std::stoi(token);
            return gameData;
        }
        else if (i > 4)
            throw std::runtime_error("PacketManager::getGameDataFromPacket: Invalid packet");
        else if (i > 4)
            break;
        data.erase(0, pos + delimiter.length());
        i++;
    }
    return gameData;
}

player_input_t PacketManager::getPlayerInputFromPacket(Packet &packet) const
{
    player_input_t playerInput;
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
            if (token != Packet::_HEADERS_INPUT[i])
                throw std::runtime_error("PacketManager::getPlayerInputFromPacket: Invalid packet");
        }
        else if (i == 1)
        {
            size_t posColon = token.find(":");
            if (posColon == std::string::npos)
                throw std::runtime_error("PacketManager::getPlayerInputFromPacket: Invalid packet");
            std::string lineBegin = token.substr(0, posColon + 1);
            if (lineBegin != Packet::_HEADERS_INPUT[i])
                throw std::runtime_error("PacketManager::getPlayerInputFromPacket: Invalid packet");
            token = token.substr(posColon + 1);
            playerInput = static_cast<player_input_t>(std::stoi(token));
            break;
        }
        else if (i > 1)
            break;
        data.erase(0, pos + delimiter.length());
        i++;
    }
    return playerInput;
}
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
