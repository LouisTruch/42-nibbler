#include "../inc/Game.hpp"
#include <ctime>
#include <iostream>
#include <ranges>
#include <unistd.h>

Game::Game(std::unique_ptr<ModeHandler> modeHandler)
    : _modeHandler(std::move(modeHandler)), _width(_modeHandler->getWidth()), _height(_modeHandler->getHeight()),
      _totalSpace(_width * _height), _gameSpeed(DEFAULT_GAME_SPEED), _turnStart(std::clock())
{
    _rng.seed(getpid());
    _libHandler = std::make_unique<LibHandler>(_width, _height);
    // +2 to includes border walls
    _graphicHandler = _libHandler->makeGraphicLib(_width + 2, _height + 2);
    _food = std::make_unique<Food>(chooseRandomFoodPos());

    if (_modeHandler->getIsSound())
    {
        _libHandler->openLib(LibHandler::SOUND, LibHandler::LIBSOUND);
        _modeHandler->setSoundHandler(_libHandler->makeSoundLib(*this));
    }
    initPlayer();
}

void Game::initPlayer()
{
    if (!_modeHandler->getIsMultiLocal())
    {
        _arrayPlayer[0] = std::make_unique<Player>(_width / 2, _height / 2, DEFAULT_PLAYER_SIZE);
        _arrayPlayer[1] = nullptr;
        return;
    }
    _arrayPlayer[0] = std::make_unique<Player>(0, _height / 2, DEFAULT_PLAYER_SIZE);
    _arrayPlayer[1] = std::make_unique<Player>(_width - 1, _height / 2, DEFAULT_PLAYER_SIZE);
}

void Game::loop(void)
{
    clock_t now;
    while (1)
    {
        _graphicHandler->registerPlayerInput();
        if (_graphicHandler->getPlayerInput(0) == QUIT)
            break;
        else if (_graphicHandler->getPlayerInput(0) >= SWAP_LIBNCURSES &&
                 _graphicHandler->getPlayerInput(0) <= SWAP_LIBRAYLIB)
            if (handleLibSwitch())
                break;

        now = std::clock();
        _turn = (now - _turnStart) / (double)CLOCKS_PER_SEC;
        if (_turn > _gameSpeed)
        {
            playersAction(MOVE);
            checkCollisions();
            _modeHandler->handleHunger(now, _arrayPlayer[0].get(), _arrayPlayer[1].get());
            checkPlayerState();
            if (_modeHandler->getIsMultiOff() && _totalSpace <= _arrayPlayer[0]->getPlayerScore())
                throwGameOverScore("Game Win");
            _graphicHandler->clearBoard();
            playersAction(DRAW);
            _graphicHandler->drawFood(_food->getPos());
            _food = _modeHandler->handleDisappearingFood(*this, std::move(_food), now);
            _turnStart = clock();
        }
    }
}

void Game::playersAction(player_action_e playerAction)
{
    for (auto &&player : _arrayPlayer)
    {
        if (player == nullptr)
            continue;

        switch (playerAction)
        {
        case MOVE:
            player->move(_graphicHandler->getPlayerInput(player->getPlayerIdx()));
            break;
        case DRAW:
            _graphicHandler->drawPlayer(*player);
            break;
        case UPDATE_SCORE:
            _modeHandler->updateScore(player->getPlayerScore(), this->_graphicHandler.get());
            break;
        }
    }
}

int Game::handleLibSwitch()
{
    // To refactor
    try
    {
        if (_graphicHandler->getPlayerInput(0) == SWAP_LIBNCURSES)
            _graphicHandler = _libHandler->switchGraphicLib(LibHandler::LIBNCURSES, std::move(_graphicHandler));
        else if (_graphicHandler->getPlayerInput(0) == SWAP_LIBSDL)
            _graphicHandler = _libHandler->switchGraphicLib(LibHandler::LIBSDL, std::move(_graphicHandler));
        else if (_graphicHandler->getPlayerInput(0) == SWAP_LIBRAYLIB)
            _graphicHandler =
                _libHandler->switchGraphicLib((LibHandler::lib_name_e)(SWAP_LIBRAYLIB - 3), std::move(_graphicHandler));
        _graphicHandler->resetPlayerInput();
        _turnStart = clock();
        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "In Game: handleLibSwitch(): " << std::endl;
        return 1;
    }
}

void Game::checkCollisions() const
{
    for (auto &&player : _arrayPlayer)
    {
        if (player == nullptr)
            continue;
        player->setPlayerCollision(STATE_NOTHING);
        auto playerHeadPoint = player->getHead();
        if (0 > playerHeadPoint->x || playerHeadPoint->x >= _width || 0 > playerHeadPoint->y ||
            playerHeadPoint->y >= _height)
        {
            player->setPlayerCollision(STATE_WALL_COLL);
            return;
        }
        if (*playerHeadPoint == _food->getPos())
        {
            player->setPlayerCollision(STATE_FOOD);
            continue;
        }
        for (auto playerBodyIt = player->getHead() + 1; playerBodyIt != player->getTail(); ++playerBodyIt)
        {
            if (*playerBodyIt == *playerHeadPoint)
            {
                player->setPlayerCollision(STATE_BODY_COLL);
                return;
            }
        }
        for (auto &&otherPlayer : _arrayPlayer)
        {
            if (otherPlayer == nullptr || otherPlayer == player)
                continue;
            for (auto playerBodyIt = otherPlayer->getHead(); playerBodyIt != otherPlayer->getTail(); ++playerBodyIt)
            {
                if (*playerBodyIt == *playerHeadPoint)
                {
                    player->setPlayerCollision(STATE_PLAYER_COLL);
                    return;
                }
            }
        }
    }
}

void Game::checkPlayerState()
{
    for (auto &&player : _arrayPlayer)
    {
        if (player == nullptr)
            continue;
        switch (player->getPlayerCollision())
        {
        case STATE_NOTHING:
            break;
        case STATE_WALL_COLL:
            throwGameOverScore("Game Over: Went in wall");
            break;
        case STATE_BODY_COLL:
            throwGameOverScore("Game Over: Went in itself");
            break;
        case STATE_FOOD:
            _modeHandler->playSound(ISoundLib::SOUND_EAT);
            player->growBody();
            player->setHungerTimer(std::clock());
            _food.reset(new Food(chooseRandomFoodPos()));
            break;
        case STATE_PLAYER_COLL:
            throwGameOverScore("Game Over: Went in other player");
            break;
        case STATE_HUNGER:
            throwGameOverScore("Game Over: Died of starvation");
            break;
        }
    }
}

point_t Game::chooseRandomFoodPos()
{
    point_t point = generateRandomPoint();
    for (int pass = 0; !isTileFree((point = generateRandomPoint())); pass++)
    {
        if (pass == 5000)
            return {-1, -1};
    }
    return point;
}

bool Game::isTileFree(point_t point)
{
    for (auto &&player : _arrayPlayer)
    {
        if (player == nullptr)
            continue;
        for (auto &playerPoint : player->getBody())
            if (playerPoint == point)
                return false;
    }
    return true;
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
    throw Game::GameOverException(str, _arrayPlayer[0]->getPlayerScore(), _width, _height);
}

Game::~Game()
{
    _libHandler->destroyGraphicLib(std::move(_graphicHandler));
    if (_modeHandler->getIsSound())
        _libHandler->destroySoundLib(_modeHandler->getSoundHandler());
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