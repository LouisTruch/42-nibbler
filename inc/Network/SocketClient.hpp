#pragma once

#include "../types.hpp"
#include "PacketManager.hpp"
#include <memory>       // std::unique_ptr
#include <netinet/in.h> // struct sockaddr_in
#include <string>       // std::string

class SocketClient
{
  public:
    SocketClient();
    ~SocketClient();

  public:
    board_size_t recvBoardData() ;

  private:
    void setupFdSocket();
    bool askIfConnectLocalIp() const noexcept;
    const std::string askForDesiredIp() const;
    void tryConnectingToIp(const std::string &ip);

  private:
    static constexpr u_int16_t _PORT = 7777;

  private:
    int _fd;
    int _byteRead;
    int _maxFds;
    std::string _localIp;
    std::unique_ptr<PacketManager> _packetManager;

  private:
    // TODO : Implement copy constructor and assignement operator
    SocketClient(const SocketClient &) = delete;
    SocketClient &operator=(const SocketClient &) = delete;
};