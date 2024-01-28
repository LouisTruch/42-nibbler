#pragma once

class Server
{
  public:
    Server();
    ~Server();
    // TODO or not: copy constructor and assignement operator
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