#pragma once

#include "../types.hpp"
#include "PacketManager.hpp"
#include <memory>       // std::unique_ptr
#include <netinet/in.h> // struct sockaddr_in
#include <string>       // std::string

class Server
{
  public:
    Server();
    ~Server();
    Server(const Server &) = delete;
    Server &operator=(const Server &) = delete;

  public:
    void waitConnection();
    void sendBoardSize(board_size_t) const;
    void sendGameData(const GameData_t &) const;
    player_input_t recvPlayerInput() const;

  private:
    void setupFdSocket();

  private:
    static constexpr u_int16_t _PORT = 7777;
    static constexpr long _TIMEOUT_SEC = 20;
    static constexpr long _TIMEOUT_USEC = 0;

  private:
    int _fd;
    struct sockaddr_in _addr;
    int _clientFd;
    int _byteRead;
    int _maxFds;
    std::unique_ptr<PacketManager> _packetManager;
};