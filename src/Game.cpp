#include "../inc/Game.hpp"
#include <ctime>
#include <iostream>
#include <ranges>
#include <unistd.h>

Game::Game(int width, int height, const ModesHandler modeHandler)
    : _width(width), _height(height), _totalSpace(width * height), _gameSpeed(DEFAULT_GAME_SPEED),
      _modesHandler(modeHandler)
{
    _rng.seed(getpid());
    _libHandler = std::make_unique<LibHandler>(_width, _height);
    // +2 to includes border walls
    _gHandler = _libHandler->makeGraphicLib(_width + 2, _height + 2);
    _player = std::make_unique<Player>(width / 2, height / 2, DEFAULT_PLAYER_SIZE);
    _food = std::make_unique<Food>(chooseRandomFoodPos());
}

void Game::loop(void)
{
    _turnStart = std::clock();
    while (1)
    {
        _gHandler->registerPlayerInput();
        if (_gHandler->getPlayerInput() == QUIT)
            break;
        else if (_gHandler->getPlayerInput() >= SWAP_LIBNCURSES && _gHandler->getPlayerInput() <= SWAP_LIBRAYLIB)
            handleLibSwitch();

        _turn = (std::clock() - _turnStart) / (double)CLOCKS_PER_SEC;
        if (_turn > _gameSpeed)
        {
            _player->move(_gHandler->getPlayerInput());
            if (checkCollision() == DEATH)
                throw Game::GameOverException("Game Over");
            _gHandler->drawPlayer(*_player);
            _gHandler->drawFood(_food->getPos());
            if (_totalSpace <= (int)_player->getBody().size())
                throw Game::GameOverException("Game Win");
            _food = _modesHandler.handleDisappearingFood(*this, std::move(_food));
            _turnStart = clock();
        }
    }
}

void Game::handleLibSwitch()
{
    // To redo
    if (_gHandler->getPlayerInput() == SWAP_LIBNCURSES)
        _gHandler = _libHandler->switchLib(LibHandler::LIBNCURSES, std::move(_gHandler));
    else if (_gHandler->getPlayerInput() == SWAP_LIBSDL)
        _gHandler = _libHandler->switchLib(LibHandler::LIBSDL, std::move(_gHandler));
    else if (_gHandler->getPlayerInput() == SWAP_LIBRAYLIB)
        _gHandler = _libHandler->switchLib(LibHandler::LIBRAYLIB, std::move(_gHandler));
    _gHandler->resetPlayerInput();
    _turnStart = clock();
}

int Game::checkCollision()
{
    auto playerHeadPoint = _player->getHead();
    if (0 > playerHeadPoint->x || playerHeadPoint->x >= _width || 0 > playerHeadPoint->y ||
        playerHeadPoint->y >= _height)
        return DEATH;

    if (*playerHeadPoint == _food->getPos())
    {
        _player->growBody();
        _food.reset(new Food(chooseRandomFoodPos()));
        _modesHandler.changeGameSpeed(SPEED_MODIFIER, *this);
        return BUFF;
    }

    for (auto playerBodyIt = _player->getHead() + 1; playerBodyIt != _player->getTail(); ++playerBodyIt)
        if (*playerBodyIt == *playerHeadPoint)
            return DEATH;

    return NOTHING;
}

bool Game::isTileFree(point_t point)
{
    for (auto &playerPoint : _player->getBody())
        if (playerPoint == point)
            return false;
    return true;
}

point_t Game::chooseRandomFoodPos()
{
    point_t point = generateRandomPoint();
    while (!isTileFree((point = generateRandomPoint())))
        ;
    return point;
}

point_t Game::generateRandomPoint()
{
    std::uniform_int_distribution<int> rngWidth(0, _width - 1);
    std::uniform_int_distribution<int> rngHeight(0, _height - 1);
    return {rngWidth(_rng), (rngHeight(_rng))};
}

double Game::getGameSpeed() const
{
    return _gameSpeed;
}

void Game::setGameSpeed(double newSpeed)
{
    _gameSpeed = newSpeed;
}

Game::~Game()
{
    _libHandler->destroyGraphicLib(std::move(_gHandler));
}

Game::Game(const Game &other) : _modesHandler(other._modesHandler)
{
    *this = other;
}

Game &Game::operator=(const Game &other)
{
    if (&other == this)
        return *this;
    _width = other._width;
    _height = other._height;
    _totalSpace = other._totalSpace;
    _rng = other._rng;
    _libHandler = std::make_unique<LibHandler>(_width, _height);
    _gHandler = _libHandler->makeGraphicLib(_width + 2, _height + 2);
    // Have to add copy _gHandler
    return *this;
}

Game::GameOverException::GameOverException(const char *msg)
{
    _msg = msg;
}

const char *Game::GameOverException::what() const throw()
{
    return _msg.c_str();
}