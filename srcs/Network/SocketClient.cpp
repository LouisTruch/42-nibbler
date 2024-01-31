#include "../../inc/Network/SocketClient.hpp"
#include "../../inc/Log/Logger.hpp"

#include <iostream> // std::cout std::cin std::cerr
#include <unistd.h> // close()
const std::string getLocalIp();
SocketClient::SocketClient() : _fd(-1), _byteRead(-1), _maxFds(-1), _localIp(getLocalIp())
{
    LOG_DEBUG("Constructing");
    try
    {
        setupFdSocket();
        if (askIfConnectLocalIp())
            tryConnectingToIp(_localIp);
        else
        {
            std::string ip = askForDesiredIp();
            tryConnectingToIp(ip);
        }
    }
    catch (const std::exception &e)
    {
        if (_fd > 0)
        {
            close(_fd);
            _fd = -1;
        }
        std::cerr << "In SocketClient.cpp: " << e.what() << std::endl;
        throw e;
    }
}

#include <sys/socket.h> // socket()
void SocketClient::setupFdSocket()
{
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_fd < 0)
        throw std::runtime_error("socket() failed");
}

bool SocketClient::askIfConnectLocalIp() const noexcept
{
    char answer;
    std::cout << "Do you want to connect to your local IP (" << _localIp << ") ? (y/n)" << std::endl;
    std::cin >> answer;
    if (answer == 'y')
        return true;
    return false;
}

const std::string SocketClient::askForDesiredIp() const
{
    std::string ip;
    std::cout << "Enter the IPv4 you want to connect to: " << std::endl;
    std::cin >> ip;
    if (!parseIp(ip))
    {
        throw std::runtime_error("Invalid IPv4");
    }
    return ip;
}

#include <arpa/inet.h> // inet_pton()
#include <cstring>     // memset()
#include <signal.h>    // signal()
void SocketClient::tryConnectingToIp(const std::string &ip)
{
    struct sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_port = htons(_PORT);
    servAddr.sin_family = AF_INET;

    LOG_DEBUG("Connecting to " + ip);
    if (inet_pton(AF_INET, ip.c_str(), &servAddr.sin_addr) <= 0)
        throw std::runtime_error("inet_pton() failed");
    if (connect(_fd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
        throw std::runtime_error("connect() failed");

    // Ignore SIGPIPE to end properly execution when server closed and recv() sends a SIGPIPE
    signal(SIGPIPE, SIG_IGN);
    LOG_DEBUG("Connection successful");
}

SocketClient::~SocketClient()
{
    LOG_DEBUG("Destructing");
    if (_fd > 0)
        close(_fd);
}

#include "../../inc/Network/Packet.hpp"
#define RECV_BUFFER_SIZE 5000
board_size_t SocketClient::recvBoardData()
{
    Packet packet(RECV_BUFFER_SIZE);
    _packetManager->recvPacket(_fd, packet);
    board_size_t boardSize = _packetManager->getBoardSizeFromPacket(packet);
    return boardSize;
}

GameData_t SocketClient::recvGameData()
{
    Packet packet(RECV_BUFFER_SIZE);
    _packetManager->recvPacket(_fd, packet);
    GameData_t gameData = _packetManager->getGameDataFromPacket(packet);
    return gameData;
}

void SocketClient::sendPlayerInput(const player_input_t &playerInput) const
{
    Packet packet(playerInput);
    _packetManager->sendPacket(_fd, packet);
}