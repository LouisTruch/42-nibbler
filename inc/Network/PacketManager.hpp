#pragma once

#include "../types.hpp"
#include "Packet.hpp"

class PacketManager
{
  public:
    PacketManager();
    ~PacketManager();
    PacketManager(const PacketManager &) = delete;
    PacketManager &operator=(const PacketManager &) = delete;

    void sendPacket(int _fd, const Packet &packet) const;
    void recvPacket(int _fd, Packet &packet) const;
    const board_size_t getBoardSizeFromPacket(Packet &packet) const;
    const GameData_t getGameDataFromPacket(Packet &packet) const;
    const player_input_t getPlayerInputFromPacket(Packet &packet) const;

  private:
};