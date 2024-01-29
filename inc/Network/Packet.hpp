#pragma once

#include "../types.hpp"
#include <string>      // std::string
#include <string_view> // std::string_view

class Packet
{
  public:
    Packet(std::string::size_type size);
    Packet(board_size_t boardSize);
    Packet(std::string_view data);
    ~Packet();
    Packet(const Packet &);
    Packet &operator=(const Packet &);

  public:
    static inline const std::string_view _MANDATORY_HEADER[1] = {
        "board_size",
    };

  public:
    const std::string &getDataToSend() const noexcept;
    std::string &getBufferRecv() noexcept;

  private:
    std::string _dataToSend;
    std::string _bufferRecv;
};