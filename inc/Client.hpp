#pragma once

#include "./Game/Game.hpp"
#include "./Interface/IGraphicLib.hpp"
#include "./Interface/ISoundLib.hpp"
#include "./Mode/ModeHandler.hpp"
#include "./Network/Server.hpp"
#include "./Network/SocketClient.hpp"
#include "LibHandler.hpp"
#include "types.hpp"

class Client
{
  public:
    Client(std::unique_ptr<LibHandler>, std::unique_ptr<Server>);
    Client(std::unique_ptr<LibHandler>, std::unique_ptr<SocketClient>);
    ~Client();
    void createGame(board_size_t boardSize, std::unique_ptr<ModeHandler> modeHandler, bool multipayer);
    void startGameLoop();
    void joinGame();

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
        SWAP_LIB_SOUND,
        DIRECTION,
        MUTE,
    } input_type_e;
    input_type_e checkPlayerInput(const player_input_t playerInput) noexcept;
    void consumePlayerInput(const player_input_t playerInput, const std::size_t playerIdx);
    void handleLibSwitch(const player_input_t playerInput);
    LibHandler::lib_graphic_e inputToLibNum(const player_input_t) noexcept;
    void handleLibSoundSwitch(const player_input_t playerInput);
    LibHandler::lib_sound_e inputToLibSoundNum(const player_input_t) noexcept;
    bool checkIfOppositeDirection(player_input_t playerInput, const std::size_t playerIdx) const noexcept;
    void render(const GameData_t &gameData) const;
    void handleSound(const GameData_t &gameData) const;
    void handleInput();

  private:
    std::unique_ptr<LibHandler> _libHandler;
    std::unique_ptr<IGraphicLib> _graphicLib;
    std::unique_ptr<Game> _game;
    std::unique_ptr<ISoundLib> _soundLib;
    std::unique_ptr<Server> _server;
    std::unique_ptr<SocketClient> _socketClient;

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