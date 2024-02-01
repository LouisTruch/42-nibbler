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
    // TODO or not: copy constructor and assignement operator

  private:
    void setupFdSocket();

  private:
    static constexpr u_int16_t _PORT = 7777;
    static constexpr long _TIMEOUT_SEC = 100;
    static constexpr long _TIMEOUT_USEC = 0;

  private:
    int _fd;
    struct sockaddr_in _addr;
    int _clientFd;
    int _byteRead;
    int _maxFds;
    std::unique_ptr<PacketManager> _packetManager;
};

// Old Class Server
// #include <netinet/in.h>
// #include <string>
// #include <string_view>
// #include <vector>

// class Server
// {

// public:
//   typedef enum
//   {
//       WAIT_CONNECTION,
//       SEND_DATA,
//       READ_DATA,
//   } server_action_e;

//   Server();
//   ~Server();
//   void waitConnection();
//   [[nodiscard("READ DATA FROM CLIENT")]] int readData();
//   void sendInitData(int, int, bool) const;
//   void sendData(std::string_view) const;
//   void printLocalIp() const;

// private:
//   int _fd;
//   struct sockaddr_in _addr;
//   int _clientFd;
//   std::vector<int> _vecClientFd;
//   static constexpr int PORT = 7777;
//   int _byteRead;
//   std::string _buffer;
//   fd_set _masterSet;
//   fd_set _selectSet;
//   int _maxFds;
//   struct timeval _timeout;
//   Server(const Server &) = delete;
//   Server &operator=(const Server &) = delete;
// };