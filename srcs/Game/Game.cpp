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
        _scoreHandler = nullptr;
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
    auto now = std::chrono::high_resolution_clock::now();
    if (getElapsedTimeInMs(now, _clock) > _gameSpeed)
    {
        LOG_DEBUG("speed: " + std::to_string(_gameSpeed));
        _clock = std::chrono::high_resolution_clock::now();
        movePlayers();
        setShouldPlayEatingSound(false);
        handleCollisions(now);
        runModesRoutine(now);
    }
}

void Game::runModesRoutine(const std::chrono::time_point<std::chrono::high_resolution_clock> &now)
{
    _modeHandler->doCheckRoutine(now);
    if (_modeHandler->getHungerState(0) == ModeHandler::CONDITION_MET)
    {
        if (_modeHandler->getHungerState(1) == ModeHandler::CONDITION_MET)
        {
            if (_p1 != nullptr)
                handleGameOver("Both players died of hunger");
        }
        handlePlayerDeath(0, "of hunger :(");
    }
    if (_modeHandler->getHungerState(1) == ModeHandler::CONDITION_MET)
    {
        if (_p1 != nullptr)
            handlePlayerDeath(1, "of hunger :(");
    }
    if (_modeHandler->getMovingFoodState() == ModeHandler::CONDITION_MET)
    {
        _food = generateFood();
    }
    if (_modeHandler->getAcceleratingSpeedState() == ModeHandler::CONDITION_MET)
    {
        _modeHandler->modifyGameSpeed(_gameSpeed);
    }
    _modeHandler->resetModeStates();
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

    if ((collision_p0 == COLLISION_ITSELF || collision_p0 == COLLISION_WALL || collision_p0 == COLLISION_OTHERPLAYER) &&
        (collision_p1 == COLLISION_ITSELF || collision_p1 == COLLISION_WALL || collision_p1 == COLLISION_OTHERPLAYER))
    {
        if (_p1 != nullptr)
            handleGameOver("Both players died");
        else
            handlePlayerDeath(0, "of " + collisionToString(collision_p0));
    }
    if ((collision_p0 == COLLISION_ITSELF || collision_p0 == COLLISION_WALL || collision_p0 == COLLISION_OTHERPLAYER))
    {
        handlePlayerDeath(0, "of " + collisionToString(collision_p0));
    }
    if ((collision_p1 == COLLISION_ITSELF || collision_p1 == COLLISION_WALL || collision_p1 == COLLISION_OTHERPLAYER))
    {
        handlePlayerDeath(1, "of " + collisionToString(collision_p1));
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
        handleGameOver("No more free space, gg :)");
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

void Game::handlePlayerDeath(int playerIdx, const std::string str) const
{
    throw Game::GameOverException("Game Over: Player " + std::to_string(playerIdx) + " died " + str);
}

void Game::handleGameOver(const std::string str) const
{
    throw Game::GameOverException("Game Over: " + str);
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

const GameData_t Game::exportData() const
{
    GameData_t gameData;
    // These should never be nullptr cause we only export in multiplayer games
    if (_p0 != nullptr)
    {
        gameData.p0.idx = _p0->_idx;
        gameData.p0.body = _p0->_body._deque;
        gameData.p0.dir = _p0->_prevDirection;
    }
    if (_p1 != nullptr)
    {
        gameData.p1 = std::make_optional(Snake_t());
        gameData.p1->idx = _p1->_idx;
        gameData.p1->body = _p1->_body._deque;
        gameData.p1->dir = _p1->_prevDirection;
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

const std::string Game::collisionToString(collision_type collision) const noexcept
{
    switch (collision)
    {
    case COLLISION_WALL:
        return "colliding with wall";
    case COLLISION_ITSELF:
        return "colliding with itself";
    case COLLISION_OTHERPLAYER:
        return "colliding with other player";
    default:
        return "UNKNOWN";
    }
}

bool Game::isScore() const noexcept
{
    if (_scoreHandler == nullptr)
        return false;
    return true;
}

int Game::getCurrentScore() const noexcept
{
    if (_scoreHandler == nullptr)
        return 0;
    return _scoreHandler->getCurrentScore();
}

int Game::getHighScore() const noexcept
{
    if (_scoreHandler == nullptr)
        return 0;
    return _scoreHandler->getHighScore();
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