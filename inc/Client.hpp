#pragma once

#include "IGraphicLib.hpp"
#include "ISoundLib.hpp"
#include "LibHandler.hpp"
#include "types.hpp"
#include <memory>

class Client
{
  public:
    Client(std::string_view);
    ~Client();
    void instantiatePtrs();
    void readData();
    void readInitData(std::string &);
    void readGameData(std::string &);
    void readMessage(std::string &);
    int handleLibSwitch();

  private:
    int _fd;
    int _width;
    int _height;
    bool _isSound;
    std::unique_ptr<LibHandler> _libHandler;
    std::unique_ptr<IGraphicLib> _graphicHandler;
    std::unique_ptr<ISoundLib> _soundHandler;
    player_input_t _playerInput;
    Client() = delete;
    Client(const Client &) = delete;
    Client &operator=(const Client &) = delete;

  private:
    static constexpr int PORT = 7777;
};