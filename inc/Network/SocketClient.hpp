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
    SocketClient(const SocketClient &) = delete;
    SocketClient &operator=(const SocketClient &) = delete;

  public:
    board_size_t recvBoardData();
    GameData_t recvGameData();
    void sendPlayerInput(const player_input_t &playerInput) const;

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
};

#include <regex>
inline bool parseIp(const std::string &input)
{
    std::regex ipv4Pattern("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
    if (std::regex_match(input, ipv4Pattern))
        return true;
    return false;
}