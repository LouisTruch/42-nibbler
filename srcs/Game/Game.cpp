#include "../../inc/Game/Game.hpp"
#include "../../inc/Log/Logger.hpp"
#include <unistd.h> // getpid()

Game::Game(board_size_t boardSize, std::unique_ptr<ModeHandler> modeHandler, bool multiplayer)
    : _modeHandler(std::move(modeHandler)), _width(boardSize.x), _height(boardSize.y),
      _totalFreeSpace((_width - 2) * (_height - 2)), _gameSpeed(_DEFAULT_GAME_SPEED_MS), _lastTurn(std::clock()),
      _p0(nullptr), _p1(nullptr), _food(nullptr), _scoreHandler(nullptr), _shouldPlayEatingSound(false)
{
    if (_lastTurn == (std::clock_t)-1)
        throw std::runtime_error("Error Game::Game(): std::clock() failed");
    _clock = std::chrono::high_resolution_clock::now();

    _rng.seed(getpid());

    // Score is only active in singleplayer games
    if (multiplayer == false)
    {
        _p0 = std::make_shared<Player>(0, _width / 2, _height / 2, _DEFAULT_PLAYER_SIZE);
        _scoreHandler = std::make_unique<Score>(_width, _height);
    }
    else if (multiplayer == true)
    {
        _p0 = std::make_shared<Player>(0, 1, _height / 2, _DEFAULT_PLAYER_SIZE);
        _p1 = std::make_shared<Player>(1, _width - 2, _height / 2, _DEFAULT_PLAYER_SIZE);
    }
    _food = generateFood();
    LOG_DEBUG("Game successfully constructed:" + getInfo());
}

Game::~Game()
{
    LOG_DEBUG("Destructing Game");
}

#include <chrono>
#include <iostream>
void Game::playTurn()
{
    // TODO : Change std::clock by std::chrono::high_resolution_clock in mods AND DELETE THIS
    // std::clock_t now = std::clock();
    // if (now == (std::clock_t)-1)
    //     throw std::runtime_error("Error Game::playTurn(): std::clock() failed");

    auto now2 = std::chrono::high_resolution_clock::now();
    if (getElapsedTimeInMs(now2, _clock) > _gameSpeed)
    {
        LOG_DEBUG("speed: " + std::to_string(_gameSpeed));
        _clock = std::chrono::high_resolution_clock::now();
        movePlayers();
        setShouldPlayEatingSound(false);
        handleCollisions(now2);
        runModesRoutine(now2);
    }
}

void Game::runModesRoutine(const std::chrono::time_point<std::chrono::high_resolution_clock> &now)
{
    size_t retMode = _modeHandler->checkRoutine(now);
    // TODO : redo this probably
    if ((retMode & 0x1) && (retMode & 0x2))
    {
        throw Game::GameOverException("Game Draw: Both players died of hunger");
    }
    else if (retMode & 0x1)
    {
        if (_p1 == nullptr)
            throw Game::GameOverException("Game Over: You died of hunger :(");
        else
            throw Game::GameOverException("Game Over: Player 0 died of hunger, player 1 wins");
    }
    else if (retMode & 0x2 && _p1 != nullptr)
    {
        throw Game::GameOverException("Game Over: Player 1 died of hunger, player 0 wins");
    }
    // Need to handle hunger, probably throw some exceptions
    if (retMode & 0x4)
    {
        _food = generateFood();
    }
    if (retMode & 0x8)
    {
        _modeHandler->modifyGameSpeed(_gameSpeed);
    }
}

std::shared_ptr<Player> Game::getP(std::size_t idx)
{
    if (idx == 0)
        return getP0();
    else if (idx == 1)
        return getP1();
    return nullptr;
}

std::shared_ptr<Player> Game::getP0()
{
    return _p0;
}

std::shared_ptr<Player> Game::getP1()
{
    return _p1;
}

std::shared_ptr<Food> Game::getFood()
{
    return _food;
}

void Game::movePlayers() noexcept
{
    if (_p0 != nullptr)
        _p0->move();
    if (_p1 != nullptr)
        _p1->move();
}

// TODO : Clean this mess
void Game::handleCollisions(const std::chrono::time_point<std::chrono::high_resolution_clock> &now)
{
    if (_p0 == nullptr)
        return;

    collision_type collision_p0 = checkPlayerCollision(_p0, _p1);
    collision_type collision_p1 = NO_COLLISION;
    if (_p1 != nullptr)
    {
        collision_p1 = checkPlayerCollision(_p1, _p0);
    }

    // TODO : Redo this
    if ((collision_p0 == COLLISION_ITSELF || collision_p0 == COLLISION_WALL || collision_p0 == COLLISION_OTHERPLAYER) &&
        (collision_p1 == COLLISION_ITSELF || collision_p1 == COLLISION_WALL || collision_p1 == COLLISION_OTHERPLAYER))
    {
        if (_p1 != nullptr)
            throw Game::GameOverException("Game Draw: Players died at the same time");
        else
            throw Game::GameOverException("Game Over: You died :(");
    }
    if ((collision_p0 == COLLISION_ITSELF || collision_p0 == COLLISION_WALL || collision_p0 == COLLISION_OTHERPLAYER))
    {
        if (_p1 != nullptr)
            throw Game::GameOverException("Game Over: Player 0 died, player 1 wins");
        else
            throw Game::GameOverException("Game Over: You died :(");
    }
    if ((collision_p1 == COLLISION_ITSELF || collision_p1 == COLLISION_WALL || collision_p1 == COLLISION_OTHERPLAYER))
    {
        throw Game::GameOverException("Game Over: Player 1 died, player 0 wins");
    }

    if (collision_p0 == COLLISION_FOOD)
    {
        setShouldPlayEatingSound(true);
        _p0->growBody();
        updateScore();
        _food = generateFood();
        _modeHandler->resetHungerTimer(0, now);
    }
    if (collision_p1 == COLLISION_FOOD)
    {
        setShouldPlayEatingSound(true);
        _p1->growBody();
        _food = generateFood();
        _modeHandler->resetHungerTimer(1, now);
    }
    size_t spaceTakenByPlayers = _p0->_body._deque.size();
    if (_p1 != nullptr)
        spaceTakenByPlayers += _p1->_body._deque.size();
    if (spaceTakenByPlayers == _totalFreeSpace)
    {
        throw Game::GameOverException("Game Draw: Players filled the entiere map, GJ :)");
    }
}

Game::collision_type Game::checkPlayerCollision(const std::shared_ptr<Player> &player,
                                                const std::shared_ptr<Player> &otherPlayer) const noexcept
{
    if (checkCollisionPlayerWall(player))
    {
        return COLLISION_WALL;
    }
    if (checkCollisionPlayerFood(player))
    {
        return COLLISION_FOOD;
    }
    if (checkCollisionPlayerItself(player))
    {
        return COLLISION_ITSELF;
    }
    if (otherPlayer != nullptr)
    {
        if (checkCollisionOtherPlayer(player, otherPlayer))
        {
            return COLLISION_OTHERPLAYER;
        }
    }

    return NO_COLLISION;
}

bool Game::checkCollisionPlayerWall(const std::shared_ptr<Player> &player) const noexcept
{
    if (player->_body._deque.front().x <= 0 || player->_body._deque.front().x >= _width - 1 ||
        player->_body._deque.front().y <= 0 || player->_body._deque.front().y >= _height - 1)
    {
        LOG_DEBUG("Player " + std::to_string(player->_idx) + " collided with wall");
        return true;
    }
    return false;
}

bool Game::checkCollisionPlayerFood(const std::shared_ptr<Player> &player) const noexcept
{
    if (player->_body._deque.front() == _food->getPos())
    {
        LOG_DEBUG("Player " + std::to_string(player->_idx) + " collided with food");
        return true;
    }
    return false;
}

bool Game::checkCollisionPlayerItself(const std::shared_ptr<Player> &player) const noexcept
{
    for (auto it = player->_body._deque.begin() + 1; it != player->_body._deque.end(); ++it)
    {
        if (player->_body._deque.front() == *it)
        {
            LOG_DEBUG("Player " + std::to_string(player->_idx) + " collided with itself");
            return true;
        }
    }
    return false;
}

bool Game::checkCollisionOtherPlayer(const std::shared_ptr<Player> &player,
                                     const std::shared_ptr<Player> &otherPlayer) const noexcept
{
    for (auto it = otherPlayer->_body._deque.begin(); it != otherPlayer->_body._deque.end(); ++it)
    {
        if (player->_body._deque.front() == *it)
        {
            return true;
        }
    }
    return false;
}

std::shared_ptr<Food> Game::generateFood()
{
    point_t pos = choseRandomUnoccupiedPoint();
    if (pos.x == -1)
        throw std::runtime_error("Error Game::generateFood(): Could not generate food");
    return std::make_unique<Food>(pos);
}

constexpr size_t MAX_PASS = 5000;
const point_t Game::choseRandomUnoccupiedPoint() noexcept
{
    point_t point;
    for (int pass = 0; isPointOccupied((point = generateRandomPoint())); pass++)
    {
        if (pass == MAX_PASS)
            return {-1, -1};
    }
    return point;
}

const point_t Game::generateRandomPoint() noexcept
{
    // +1 to avoid spawning Food on border walls same for -2
    std::uniform_int_distribution<int> rngWidth(1, _width - 2);
    std::uniform_int_distribution<int> rngHeight(1, _height - 2);
    return {rngWidth(_rng), rngHeight(_rng)};
}

bool Game::isPointOccupied(const point_t &point) const noexcept
{
    if (_p0 != nullptr && _p0->doesPointIntersectP(point))
        return true;
    if (_p1 != nullptr && _p1->doesPointIntersectP(point))
        return true;
    return false;
}

void Game::updateScore() noexcept
{
    if (_scoreHandler == nullptr)
        return;
    _scoreHandler->setCurrentScore(_p0->_body._deque.size());
}

void Game::setShouldPlayEatingSound(bool shouldPlay) noexcept
{
    _shouldPlayEatingSound = shouldPlay;
}

bool Game::getShouldPlayEatingSound() const noexcept
{
    return _shouldPlayEatingSound;
}

std::string Game::getInfo() const noexcept
{
#ifndef DEBUG
    return "";
#else
    std::string info = " Width:";
    info += std::to_string(_width);
    info += " Height:";
    info += std::to_string(_height);
    info += " GameSpeed:";
    info += std::to_string(_gameSpeed);
    info += " Multiplayer:";
    info += (_p1 != nullptr) ? "On" : "Off";
    return info;
#endif
}

// TODO : Need to add direction to gameData because of raylib
// TODO : Add sound somehow
const GameData_t Game::exportData() const
{
    GameData_t gameData;
    // These should never be nullptr cause we only export in multiplayer games
    if (_p0 != nullptr)
    {
        gameData.p0.idx = _p0->_idx;
        gameData.p0.body = _p0->_body._deque;
        gameData.p0.dir = _p0->_nextDirection;
    }
    if (_p1 != nullptr)
    {
        gameData.p1 = std::make_optional(Snake_t());
        gameData.p1->idx = _p1->_idx;
        gameData.p1->body = _p1->_body._deque;
        gameData.p1->dir = _p1->_nextDirection;
    }
    else
    {
        gameData.p1 = std::nullopt;
    }
    if (_food != nullptr)
    {
        gameData.food = _food->getPos();
    }
    gameData.playEatingSound = _shouldPlayEatingSound;
    return gameData;
}

Game::GameOverException::GameOverException(const std::string msg) noexcept : msg(msg)
{
}

Game::GameOverException::~GameOverException() throw()
{
}

const char *Game::GameOverException::what() const noexcept
{
    return msg.c_str();
}