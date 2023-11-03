#include "../inc/Game.hpp"
#include <ctime>
#include <iostream>
#include <ranges>
#include <unistd.h>

Game::Game(int width, int height, std::unique_ptr<ModeHandler> modeHandler)
    : _width(width), _height(height), _totalSpace(width * height), _gameSpeed(DEFAULT_GAME_SPEED),
      _modeHandler(std::move(modeHandler)), _turnStart(std::clock())
{
    _rng.seed(getpid());
    _libHandler = std::make_unique<LibHandler>(_width, _height);
    // +2 to includes border walls
    _graphicHandler = _libHandler->makeGraphicLib(_width + 2, _height + 2);
    _player = std::make_unique<Player>(width / 2, height / 2, DEFAULT_PLAYER_SIZE);
    _food = std::make_unique<Food>(chooseRandomFoodPos());

    // Create Sound Handler in ModeHandler if needed
    if (_modeHandler->getIsSound())
    {
        _libHandler->openLib(LibHandler::SOUND, LibHandler::LIBSOUND);
        _modeHandler->setSoundHandler(_libHandler->makeSoundLib(*this));
    }
}

void Game::loop(void)
{
    clock_t now;
    while (1)
    {
        _graphicHandler->registerPlayerInput();
        if (_graphicHandler->getPlayerInput() == QUIT)
            break;
        else if (_graphicHandler->getPlayerInput() >= SWAP_LIBNCURSES &&
                 _graphicHandler->getPlayerInput() <= SWAP_LIBRAYLIB)
            handleLibSwitch();

        now = std::clock();
        _turn = (now - _turnStart) / (double)CLOCKS_PER_SEC;
        if (_turn > _gameSpeed)
        {
            if (_modeHandler->handleHunger(now))
                throwGameOverScore("Game Over: Died of hunger");
            _player->move(_graphicHandler->getPlayerInput());
            game_collision_e collision = checkCollision();
            if (collision == DEATH_WALL)
                throwGameOverScore("Game Over: Went in a wall");
            else if (collision == DEATH_BODY)
                throwGameOverScore("Game Over: Went in player");
            else if (collision == EAT)
            {
                _player->growBody();
                _food.reset(new Food(chooseRandomFoodPos()));
                _modeHandler->playSound(ISoundLib::EATING);
                _modeHandler->changeGameSpeed(SPEED_MODIFIER, *this);
                _modeHandler->resetHungerTimer(now);
            }
            if (_totalSpace <= _player->getPlayerScore())
                throwGameOverScore("Game Win");
            _graphicHandler->drawPlayer(*_player);
            _graphicHandler->drawFood(_food->getPos());
            _modeHandler->updateScore(_player->getPlayerScore(), this->_graphicHandler.get());
            _food = _modeHandler->handleDisappearingFood(*this, std::move(_food), now);
            _turnStart = clock();
        }
    }
}

void Game::handleLibSwitch()
{
    // To redo
    if (_graphicHandler->getPlayerInput() == SWAP_LIBNCURSES)
        _graphicHandler = _libHandler->switchGraphicLib(LibHandler::LIBNCURSES, std::move(_graphicHandler));
    else if (_graphicHandler->getPlayerInput() == SWAP_LIBSDL)
        _graphicHandler = _libHandler->switchGraphicLib(LibHandler::LIBSDL, std::move(_graphicHandler));
    else if (_graphicHandler->getPlayerInput() == SWAP_LIBRAYLIB)
        _graphicHandler = _libHandler->switchGraphicLib(LibHandler::LIBRAYLIB, std::move(_graphicHandler));
    _graphicHandler->resetPlayerInput();
    _turnStart = clock();
}

Game::game_collision_e Game::checkCollision()
{
    auto playerHeadPoint = _player->getHead();
    if (0 > playerHeadPoint->x || playerHeadPoint->x >= _width || 0 > playerHeadPoint->y ||
        playerHeadPoint->y >= _height)
        return DEATH_WALL;

    if (*playerHeadPoint == _food->getPos())
        return EAT;

    for (auto playerBodyIt = _player->getHead() + 1; playerBodyIt != _player->getTail(); ++playerBodyIt)
        if (*playerBodyIt == *playerHeadPoint)
            return DEATH_BODY;

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

void Game::throwGameOverScore(std::string_view str) const
{
    throw Game::GameOverException(str, _player->getPlayerScore(), _width, _height);
}

Game::~Game()
{
    _libHandler->destroyGraphicLib(std::move(_graphicHandler));
    // _libHandler->destroySoundLib(_modeHandler->getSoundHandler());
}

Game::Game(const Game &other)
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
    _graphicHandler = _libHandler->makeGraphicLib(_width + 2, _height + 2);
    _modeHandler = std::make_unique<ModeHandler>();
    return *this;
}

Game::GameOverException::GameOverException(std::string_view str, int playerScore, int width, int height)
{
    _msg = str;
    _msg += PLAYER_SCORE_STR;
    _msg += std::to_string(playerScore);
    _msg += MAP_WIDTH_STR;
    _msg += std::to_string(width);
    _msg += MAP_HEIGHT_STR;
    _msg += std::to_string(height);
}

const char *Game::GameOverException::what() const throw()
{
    return _msg.c_str();
}