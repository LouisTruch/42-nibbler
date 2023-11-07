#include "../inc/Server.hpp"
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

Server::Server()
{
    if ((_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        throw std::runtime_error("socket()");

    int yes = 1;
    if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
        throw std::runtime_error("setsocketopt SO_REUSEADDR()");

    memset(&_addr, 0, sizeof(_addr));
    _addr.sin_port = htons(PORT);
    _addr.sin_family = AF_INET;
    if (bind(_fd, (struct sockaddr *)&_addr, sizeof(_addr)) == -1)
        throw std::runtime_error("bind()");

    if (listen(_fd, 10) == -1)
        throw std::runtime_error("listen()");
    _clientFd = 0;
    _byteRead = 1;
    _maxFds = _fd;
    FD_ZERO(&_masterSet);
    FD_SET(_fd, &_masterSet);
    _timeout.tv_sec = 0;
    _timeout.tv_usec = 0;
}

void Server::waitConnection()
{
    std::cout << "Waiting for another player to join" << std::endl;
    _selectSet = _masterSet;
    if (select(_maxFds + 1, &_selectSet, NULL, NULL, NULL) < 0)
        throw std::runtime_error("WaitConnection: select()");

    for (int i = 0; i <= _maxFds; i++)
    {
        if (FD_ISSET(i, &_selectSet) && i == _fd)
        {
            socklen_t addrLen = sizeof(_addr);
            _clientFd = accept(_fd, (struct sockaddr *)&_addr, &addrLen);
            if (_clientFd == -1)
                throw std::runtime_error("accept()");
            FD_SET(_clientFd, &_masterSet);
            _vecClientFd.push_back(_clientFd);
            _maxFds = _clientFd > _maxFds ? _clientFd : _maxFds;
            std::cout << "A player joined, starting game...\n";
        }
    }
}

void Server::sendInitData(int width, int height) const
{
    std::string buffer;
    buffer += std::to_string(width);
    buffer.append(" ");
    buffer += std::to_string(height);
    send(_clientFd, buffer.c_str(), buffer.size(), 0);
}

// Return client input directly in int form
int Server::readData()
{
    _selectSet = _masterSet;
    if (select(_maxFds + 1, &_selectSet, NULL, NULL, &_timeout) < 0)
        throw std::runtime_error("ReadData: select()");
    std::string buffer;
    buffer.resize(20);
    for (int i = 0; i <= _maxFds; i++)
    {
        if (FD_ISSET(i, &_selectSet))
        {
            _byteRead = recv(_clientFd, &buffer[0], buffer.size(), 0);
            if (_byteRead <= 0)
                throw std::runtime_error("ReadData: recv()");
            return (int)std::strtod(buffer.c_str(), NULL);
        }
    }
    return 0;
}

void Server::sendGameData(std::string_view gameData) const
{
    if (send(_clientFd, gameData.data(), gameData.size() + 1, 0) == -1)
    {
        // handle error
    }
}

Server::~Server()
{
    close(_fd);
    close(_clientFd);
}