#pragma once

#include <netinet/in.h>
#include <string>
#include <string_view>
#include <vector>

class Server
{
  public:
    typedef enum
    {
        WAIT_CONNECTION,
        SEND_INIT_DATA,
        SEND_GAME_DATA,
        READ_CLIENT_DATA,
    } server_action_e;

    Server();
    ~Server();
    void waitConnection();
    [[nodiscard("READ DATA FROM CLIENT")]] int readData();
    void sendInitData(int, int, bool) const;
    void sendGameData(std::string_view) const;

  private:
    int _fd;
    struct sockaddr_in _addr;
    int _clientFd;
    std::vector<int> _vecClientFd;
    static constexpr int PORT = 7777;
    int _byteRead;
    std::string _buffer;
    fd_set _masterSet;
    fd_set _selectSet;
    int _maxFds;
    struct timeval _timeout;
    Server(const Server &) = delete;
    Server &operator=(const Server &) = delete;
};