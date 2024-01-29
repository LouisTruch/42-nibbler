#include "../../inc/Network/Server.hpp"
#include "../../inc/Log/Logger.hpp"
#include <cstring>      // memset()
#include <sys/socket.h> // socket() bind() listen() accept()

Server::Server() : _fd(-1), _clientFd(0), _byteRead(1), _maxFds(-1)
{
    LOG_DEBUG("Constructing");
    setupFdSocket();
}

void Server::setupFdSocket()
{
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_fd < 0)
        throw std::runtime_error("socket() failed");

    int yes = 1;
    if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
        throw std::runtime_error("setsocketopt SO_REUSEADDR() failed");

    memset(&_addr, 0, sizeof(_addr));
    _addr.sin_port = htons(_PORT);
    _addr.sin_family = AF_INET;
    if (bind(_fd, (struct sockaddr *)&_addr, sizeof(_addr)) == -1)
        throw std::runtime_error("bind() failed");

    if (listen(_fd, 10) == -1)
        throw std::runtime_error("listen() failed");
    _maxFds = _fd;
}

#include <unistd.h> // close()
Server::~Server()
{
    LOG_DEBUG("Destructing");
    if (_fd != -1)
        close(_fd);
    if (_clientFd != -1)
        close(_clientFd);
}

// TODO : Find a way to wake from a select call without using a timeout
#include <iostream> // std::cout
const std::string getLocalIp();
void Server::waitConnection()
{
    std::cout << "Waiting for another player to join on IP: " << getLocalIp() << std::endl;
    fd_set selectSet, masterSet;
    FD_ZERO(&masterSet);
    FD_SET(_fd, &masterSet);
    // Not needed because we only accept one client
    selectSet = masterSet;
    struct timeval timeout = {.tv_sec = _TIMEOUT_SEC, .tv_usec = _TIMEOUT_USEC};
    int ret = select(_maxFds + 1, &selectSet, NULL, NULL, &timeout);
    if (ret < 0)
    {
        throw std::runtime_error("WaitConnection: select() failed");
    }
    else if (ret == 0)
    {
        throw std::runtime_error("WaitConnection: select() timed out");
    }

    for (int i = 0; i <= _maxFds; i++)
    {
        if (FD_ISSET(i, &selectSet) && i == _fd)
        {
            socklen_t addrLen = sizeof(_addr);
            _clientFd = accept(_fd, (struct sockaddr *)&_addr, &addrLen);
            if (_clientFd == -1)
                throw std::runtime_error("accept() failed");
            FD_SET(_clientFd, &masterSet);
            // _vecClientFd.push_back(_clientFd);
            _maxFds = _clientFd > _maxFds ? _clientFd : _maxFds;
            LOG_DEBUG("A player joined, starting game...");
        }
    }
}

void Server::sendBoardSize(board_size_t boardSize) const
{
    _packetManager->sendPacket(_clientFd, Packet(boardSize));
}

void Server::sendGameData(std::string_view data) const
{
    _packetManager->sendPacket(_clientFd, Packet(data));
}

player_input_t Server::recvPlayerInput() const
{
    return _packetManager->recvPacket(_clientFd);
}

// Old implementation of Server class
// #include <cstring>
// #include <iostream>
// #include <stdexcept>
// #include <sys/select.h>
// #include <sys/socket.h>
// #include <unistd.h>

// Server::Server()
// {
//     if ((_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
//         throw std::runtime_error("socket()");

//     int yes = 1;
//     if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
//         throw std::runtime_error("setsocketopt SO_REUSEADDR()");

//     memset(&_addr, 0, sizeof(_addr));
//     _addr.sin_port = htons(PORT);
//     _addr.sin_family = AF_INET;
//     if (bind(_fd, (struct sockaddr *)&_addr, sizeof(_addr)) == -1)
//         throw std::runtime_error("bind()");

//     if (listen(_fd, 10) == -1)
//         throw std::runtime_error("listen()");
//     _clientFd = 0;
//     _byteRead = 1;
//     _maxFds = _fd;
//     FD_ZERO(&_masterSet);
//     FD_SET(_fd, &_masterSet);
//     _timeout.tv_sec = 0;
//     _timeout.tv_usec = 0;
// }

// void Server::waitConnection()
// {
//     std::cout << "Waiting for another player to join";
//     printLocalIp();
//     _selectSet = _masterSet;
//     if (select(_maxFds + 1, &_selectSet, NULL, NULL, NULL) < 0)
//         throw std::runtime_error("WaitConnection: select()");

//     for (int i = 0; i <= _maxFds; i++)
//     {
//         if (FD_ISSET(i, &_selectSet) && i == _fd)
//         {
//             socklen_t addrLen = sizeof(_addr);
//             _clientFd = accept(_fd, (struct sockaddr *)&_addr, &addrLen);
//             if (_clientFd == -1)
//                 throw std::runtime_error("accept()");
//             FD_SET(_clientFd, &_masterSet);
//             _vecClientFd.push_back(_clientFd);
//             _maxFds = _clientFd > _maxFds ? _clientFd : _maxFds;
//             std::cout << "A player joined, starting game...\n";
//         }
//     }
// }

// // Return client input directly in int form because it is the only thing we need from client
// int Server::readData()
// {
//     _selectSet = _masterSet;
//     if (select(_maxFds + 1, &_selectSet, NULL, NULL, &_timeout) < 0)
//         throw std::runtime_error("ReadData: select()");
//     std::string buffer;
//     buffer.resize(20);
//     for (int i = 0; i <= _maxFds; i++)
//     {
//         if (FD_ISSET(i, &_selectSet))
//         {
//             _byteRead = recv(_clientFd, &buffer[0], buffer.size(), 0);
//             if (_byteRead <= 0)
//                 throw std::runtime_error("ReadData: recv()");
//             return (int)std::strtod(buffer.c_str(), NULL);
//         }
//     }
//     return 0;
// }

// void Server::sendData(std::string_view data) const
// {
//     if (send(_clientFd, data.data(), data.size() + 1, 0) == -1)
//         throw std::runtime_error("In Server::sendData(): send()");
// }

// Server::~Server()
// {
//     close(_fd);
//     close(_clientFd);
// }