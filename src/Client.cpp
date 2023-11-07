#include "../inc/Client.hpp"
#include <arpa/inet.h>
#include <csignal>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

Client::Client(const char *ip)
{
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if ((_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        throw std::runtime_error("In Client(): socket() error");

    struct sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);

    // Change address below by argv1
    if (inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr) <= 0)
        throw std::runtime_error("In Client(): inet_pton() error");
    if (connect(_fd, (struct sockaddr *)&servAddr, sizeof(servAddr)))
        throw std::runtime_error("In Client(): connect() error");

    // Ignore SIGPIPE to end properly execution when server closed
    signal(SIGPIPE, SIG_IGN);
}

void Client::readInitData()
{
    char tmpRecvBuffer[200] = {0};
    if (recv(_fd, tmpRecvBuffer, sizeof(tmpRecvBuffer), 0) < 0)
        throw std::runtime_error("In Client::readInitData(): recv() error");
    std::string buffer(tmpRecvBuffer);
    _width = std::stoi(buffer, nullptr, 10);
    buffer.erase(0, 2);
    _height = std::stoi(buffer, nullptr, 10);
}

void Client::instantiatePtrs()
{
    _libHandler = std::make_unique<LibHandler>(_width, _height);
    _graphicHandler = _libHandler->makeGraphicLib(_width + 2, _height + 2);
    // if sound make soundhandler
}

void Client::readData()
{
    std::string bufferRecv, bufferSend;
    bufferRecv.resize(5000);
    int byteRead = 1;
    int byteSent;
    while (byteRead > 0)
    {
        byteRead = recv(_fd, &bufferRecv[0], bufferRecv.size(), 0);
        if (byteRead < 0)
        {
            std::cerr << "recv error\n";
            break;
        }

        try
        {
            constructDrawables(bufferRecv);
        }
        catch (const std::exception &e)
        {
            std::cerr << "In Client::readData(): " << e.what() << std::endl;
        }
        _graphicHandler->registerPlayerInput();
        if (_graphicHandler->getPlayerInput(0) == QUIT)
        {
            std::cout << "ici\n";
            break;
        }
        else if (_graphicHandler->getPlayerInput(0) >= SWAP_LIBNCURSES &&
                 _graphicHandler->getPlayerInput(0) <= SWAP_LIBRAYLIB)
        {
            if (handleLibSwitch())
                break;
        }
        else
        {
            bufferSend += std::to_string(_graphicHandler->getPlayerInput(0));
            byteSent = send(_fd, bufferSend.c_str(), 2, 0);
            bufferSend.clear();
        }
    }
}

// Split this
void Client::constructDrawables(std::string buffer)
{
    if (buffer[0] != 'p')
        return;
    bool isP0 = true;
    std::deque<point_t> vecPlayer0Body;
    int player0Dir;
    std::deque<point_t> vecPlayer1Body;
    int player1Dir;
    buffer.erase(0, 2);
    int i;
    for (i = 0; buffer[i]; i++)
    {
        if (buffer[i] == 'p')
        {
            isP0 = false;
            i += 2;
            if (i > (int)buffer.size())
                break;
        }
        if (isP0 && buffer[i] == 'd' && buffer[i + 1])
        {
            player0Dir = std::stoi(&buffer[i + 1], 0, 10);
            i += 3;
            if (i > (int)buffer.size())
                break;
        }

        if (!isP0 && buffer[i] == 'd' && buffer[i + 1])
        {
            player1Dir = std::stoi(&buffer[i + 1], 0, 10);
            i += 3;
            if (i > (int)buffer.size())
                break;
        }

        if (isdigit(buffer[i]))
        {
            point_t bodyPart;
            bodyPart.x = std::stoi(&buffer[i], 0, 10);
            i += 2;
            if (i > (int)buffer.size())
                break;
            bodyPart.y = std::stoi(&buffer[i], 0, 10);
            i += 2;
            if (isP0)
                vecPlayer0Body.push_back(bodyPart);
            else
                vecPlayer1Body.push_back(bodyPart);
            if (i > (int)buffer.size())
                break;
        }

        if (buffer[i] == 'f')
            break;
    }
    buffer.erase(0, i + 2);
    point_t foodPoint;
    foodPoint.x = std::stoi(buffer, 0, 10);
    buffer.erase(0, 3);
    foodPoint.y = std::stoi(buffer, 0, 10);
    _graphicHandler->clearBoard();
    // Add player index to player constructor
    _graphicHandler->drawPlayer(Player(vecPlayer0Body, player0Dir));
    _graphicHandler->drawPlayer(Player(vecPlayer1Body, player1Dir));
    _graphicHandler->drawFood(foodPoint);
}

int Client::handleLibSwitch()
{
    // To refactor
    try
    {
        if (_graphicHandler->getPlayerInput(0) == SWAP_LIBNCURSES)
            _graphicHandler = _libHandler->switchGraphicLib(LibHandler::LIBNCURSES, std::move(_graphicHandler));
        else if (_graphicHandler->getPlayerInput(0) == SWAP_LIBSDL)
            _graphicHandler = _libHandler->switchGraphicLib(LibHandler::LIBSDL, std::move(_graphicHandler));
        else if (_graphicHandler->getPlayerInput(0) == SWAP_LIBRAYLIB)
            _graphicHandler =
                _libHandler->switchGraphicLib((LibHandler::lib_name_e)(SWAP_LIBRAYLIB - 3), std::move(_graphicHandler));
        _graphicHandler->resetPlayerInput();
        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "In Game: handleLibSwitch(): " << std::endl;
        return 1;
    }
}

Client::~Client()
{
    close(_fd);
    _libHandler->destroyGraphicLib(std::move(_graphicHandler));
    // if (_modeHandler->getIsSound())
    // _libHandler->destroySoundLib(_modeHandler->getSoundHandler());
}