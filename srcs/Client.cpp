#include "../inc/Client.hpp"
#include "../inc/Log/Logger.hpp"

#include <memory> // std::unique_ptr

// Constructor for multiplayer local and multiplayer online when hosting and singleplayer
Client::Client(std::unique_ptr<LibHandler> libHandler, std::unique_ptr<Server> server)
    : _libHandler(std::move(libHandler)), _graphicLib(_libHandler->makeGraphicLib()), _game(nullptr),
      _soundLib(_libHandler->makeSoundLib()), _server(std::move(server)), _socketClient(nullptr)
{
    LOG_DEBUG("Constructing Client");
    if (_server != nullptr)
    {
        _server->waitConnection();
        _server->sendBoardSize(_libHandler->getBoardSize());
    }
}

// Constructor for multiplayer online when joining
Client::Client(std::unique_ptr<LibHandler> libHandler, std::unique_ptr<SocketClient> socketClient)
    : _libHandler(std::move(libHandler)), _graphicLib(_libHandler->makeGraphicLib()), _game(nullptr),
      _soundLib(_libHandler->makeSoundLib()), _server(nullptr), _socketClient(std::move(socketClient))
{
    LOG_DEBUG("Constructing Client");
}

Client::~Client()
{
    LOG_DEBUG("Destructing Client");
}

void Client::createGame(board_size_t boardSize, std::unique_ptr<ModeHandler> modeHandler, bool multiplayer)
{
    _game = std::make_unique<Game>(boardSize, std::move(modeHandler), multiplayer);
    if (_server != nullptr)
    {
        _server->sendGameData(_game->exportData());
    }
}

#include <chrono>   // std::chrono
#include <iostream> // std::cout
#include <thread>   // std::this_thread::sleep_for
void Client::startGameLoop()
{
    // auto test1 = std::chrono::high_resolution_clock::now();
    // size_t nbIterationPerSec = 0;
    if (_graphicLib == nullptr)
        throw std::runtime_error("In Client::startGame(): _graphicLib is nullptr");
    while (1)
    {
        try
        {
            handleInput();
            _game->playTurn();
            GameData_t gameData = _game->exportData();
            if (_server != nullptr)
            {
                _server->sendGameData(gameData);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            render(gameData);
            handleSound(gameData);
        }
        catch (const Game::GameOverException &e)
        {
            std::cout << e.what() << std::endl;
            break;
        }
        // nbIterationPerSec++;
        // auto test2 = std::chrono::high_resolution_clock::now();
        // if (std::chrono::duration_cast<std::chrono::seconds>(test2 - test1).count() >= 1)
        // {
        //     std::cout << "Iter Per sec: " << nbIterationPerSec << std::endl;
        //     test1 = std::chrono::high_resolution_clock::now();
        //     nbIterationPerSec = 0;
        // }
    }
    LOG_DEBUG("Exiting Client::startGame()");
}

void Client::joinGame()
{
    if (_graphicLib == nullptr)
        throw std::runtime_error("In Client::joinGame(): _graphicLib is nullptr");
    while (1)
    {
        _graphicLib->registerPlayerInput();
        player_input_t input = _graphicLib->getPlayerInput(0);
        input_type_e inputType = checkPlayerInput(input);
        if (inputType == QUIT)
            throw std::runtime_error("In Client:: INPUT_QUIT received");
        else if (inputType == SWAP_LIB)
        {
            handleLibSwitch(input);
        }
        else if (inputType == MUTE)
        {
            if (_soundLib != nullptr)
                _soundLib->inverseMute();
        }
        _socketClient->sendPlayerInput(input);
        _graphicLib->resetPlayerInput();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        GameData_t gameData = _socketClient->recvGameData();
        render(gameData);
        handleSound(gameData);
    }
}

void Client::handleInput()
{
    _graphicLib->registerPlayerInput();
    consumePlayerInput(_graphicLib->getPlayerInput(0), 0);
    if (_game->getP1() != nullptr)
    {
        player_input_t inputP1 = INPUT_DEFAULT;
        if (_server != nullptr)
            inputP1 = _server->recvPlayerInput();
        else
            inputP1 = _graphicLib->getPlayerInput(1);
        consumePlayerInput(inputP1, 1);
    }
    _graphicLib->resetPlayerInput();
}

void Client::consumePlayerInput(const player_input_t playerInput, const std::size_t playerIdx)
{
    if (_game->getP(playerIdx) == nullptr)
        return;

    if (playerInput == INPUT_DEFAULT)
        return;
    input_type_e inputType = checkPlayerInput(playerInput);
    if (inputType == QUIT)
        throw std::runtime_error("In Client:: INPUT_QUIT received");
    else if (inputType == SWAP_LIB && playerIdx == 0)
    {
        handleLibSwitch(playerInput);
    }
    else if (inputType == DIRECTION)
    {
        if (!checkIfOppositeDirection(playerInput, playerIdx))
            _game->getP(playerIdx)->setNextDirection(playerInput);
    }
    else if (inputType == MUTE && playerIdx == 0)
    {
        if (_soundLib != nullptr)
            _soundLib->inverseMute();
    }
}

Client::input_type_e Client::checkPlayerInput(const player_input_t playerInput) noexcept
{
    switch (playerInput)
    {
    case INPUT_QUIT:
        return QUIT;

#ifdef DEBUG
    case INPUT_SWAP_LIBDEBUG:
        [[fallthrough]];
#endif
    case INPUT_SWAP_LIBNCURSES:
        [[fallthrough]];
    case INPUT_SWAP_LIBSDL:
        [[fallthrough]];
    case INPUT_SWAP_LIBRAYLIB:
        return SWAP_LIB;

    case INPUT_LEFT:
        [[fallthrough]];
    case INPUT_RIGHT:
        [[fallthrough]];
    case INPUT_UP:
        [[fallthrough]];
    case INPUT_DOWN:
        return DIRECTION;

    case INPUT_MUTE:
        return MUTE;

    case INPUT_DEFAULT:
        [[fallthrough]];
    default:
        break;
    }
    return UNHANDLED;
}

LibHandler::lib_graphic_e Client::inputToLibNum(const player_input_t input) noexcept
{
    switch (input)
    {
    case INPUT_SWAP_LIBNCURSES:
        return LibHandler::LIBNCURSES;
    case INPUT_SWAP_LIBSDL:
        return LibHandler::LIBSDL;
    case INPUT_SWAP_LIBRAYLIB:
        return LibHandler::LIBRAYLIB;
#ifdef DEBUG
    case INPUT_SWAP_LIBDEBUG:
        return LibHandler::LIBDEBUG;
#endif
    default:
        return LibHandler::NO_LIB;
    }
}

void Client::handleLibSwitch(const player_input_t playerInput)
{
    LibHandler::lib_graphic_e libNum = inputToLibNum(playerInput);
    _graphicLib = _libHandler->switchGraphicLib(libNum, std::move(_graphicLib));
}

bool Client::checkIfOppositeDirection(const player_input_t playerInput, const std::size_t playerIdx) const noexcept
{
    if (playerInput + (player_input_t)_game->getP(playerIdx)->getPrevDirection() == 0)
        return true;
    return false;
}

void Client::render(const GameData_t &gameData) const
{
    _graphicLib->clearBoard();
    if (gameData.p0.body.size())
        _graphicLib->drawPlayer(gameData.p0);
    if (gameData.p1.has_value())
        _graphicLib->drawPlayer(gameData.p1.value());
    _graphicLib->drawFood(gameData.food);
    if (_game != nullptr && _game->getP1() == nullptr)
        _graphicLib->drawScores(_game->getCurrentScore(), _game->getHighScore());
}

void Client::handleSound(const GameData_t &gameData) const
{
    if (_soundLib != nullptr)
    {
        if (gameData.playEatingSound)
        {
            _soundLib->playSound(ISoundLib::SOUND_EAT);
        }
    }
}