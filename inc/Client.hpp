#pragma once

#include "./Interface/IGraphicLib.hpp"
#include "./Interface/ISoundLib.hpp"
#include "./Mode/ModeHandler.hpp"
#include "Game.hpp"
#include "LibHandler.hpp"
#include "types.hpp"

class Client
{
  public:
    Client(std::unique_ptr<ModeHandler>, std::unique_ptr<LibHandler>);
    ~Client();
    void createGame(board_size_t boardSize, bool multipayer);
    void startGame();

  private:
    Client() = delete;
    // Delete those because of unique_ptr
    Client(const Client &) = delete;
    Client &operator=(const Client &) = delete;

    typedef enum
    {
        UNHANDLED,
        QUIT,
        SWAP_LIB,
        DIRECTION,
        MUTE,
    } input_type_e;
    input_type_e checkPlayerInput(const player_input_t playerInput) noexcept;
    LibHandler::lib_graphic_e inputToLibNum(const player_input_t) noexcept;
    void consumePlayerInput(const player_input_t playerInput, const std::size_t playerIdx);
    void handleLibSwitch(const player_input_t playerInput);
    bool checkIfOppositeDirection(player_input_t playerInput, const std::size_t playerIdx) const noexcept;
    void render() const;
    void handleSound() const;

  private:
    std::unique_ptr<ModeHandler> _modeHandler;
    std::unique_ptr<LibHandler> _libHandler;
    std::unique_ptr<IGraphicLib> _graphicLib;
    std::unique_ptr<Game> _game;
    std::unique_ptr<ISoundLib> _soundLib;

    // Old Client class
    // public:
    //   Client(std::string_view);
    //   ~Client();
    //   void instantiatePtrs();
    //   void readData();
    //   void readInitData(std::string &);
    //   void readGameData(std::string &);
    //   void readMessage(std::string &);
    //   int handleLibSwitch();

    // private:
    //   int _fd;
    //   int _width;
    //   int _height;
    //   bool _isSound;
    //   std::unique_ptr<LibHandler> _libHandler;
    //   std::unique_ptr<IGraphicLib> _graphicHandler;
    //   std::unique_ptr<ISoundLib> _soundHandler;
    //   player_input_t _playerInput;
    //   Client() = delete;
    //   Client(const Client &) = delete;
    //   Client &operator=(const Client &) = delete;

    // private:
    //   static constexpr int PORT = 7777;
};