#pragma once

#include "../types.hpp"
#include <string>      // std::string
#include <string_view> // std::string_view

class Packet
{
  public:
    Packet(std::string::size_type size);
    Packet(const board_size_t &boardSize);
    Packet(const GameData_t &gameData);
    Packet(const player_input_t &playerInput);
    ~Packet();
    Packet(const Packet &);
    Packet &operator=(const Packet &);
    Packet() = delete;

  public:
    static inline const std::string_view _HEADERS_BOARDSIZE[3] = {
        "board_size",
        "width:",
        "height:",
    };
    static inline const std::string_view _HEADERS_GAMEDATA[4] = {
        "game_data",
        "player0:",
        "player1:",
        "food:",
    };
    static inline const std::string_view _HEADERS_INPUT[2] = {
        "player_input",
        "direction:",
    };

  public:
    const std::string &getDataToSend() const noexcept;
    std::string &getBufferRecv() noexcept;

  private:
    std::string _dataToSend;
    std::string _bufferRecv;
};