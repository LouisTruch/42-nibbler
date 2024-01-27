#include "../inc/Game.hpp"
#include "../inc/Log/Logger.hpp"
#include <unistd.h> // getpid()

Game::Game(board_size_t boardSize, std::unique_ptr<ModeHandler> modeHandler)
    : _modeHandler(std::move(modeHandler)), _width(boardSize.x), _height(boardSize.y),
      _totalFreeSpace((_width - 2) * (_height - 2)), _gameSpeed(DEFAULT_GAME_SPEED_MS), _lastTurn(std::clock()),
      _p0(nullptr), _p1(nullptr), _food(nullptr)
{
    LOG_DEBUG("Constructing Game");
    if (_lastTurn == (std::clock_t)-1)
        throw std::runtime_error("Error Game::Game(): std::clock() failed");

    _rng.seed(getpid());

    _p0 = std::make_shared<Player>(0, _width / 2, _height / 2, DEFAULT_PLAYER_SIZE);
    _food = generateFood();
    // _libHandler = std::make_unique<LibHandler>(_width, _height);
    // +2 to includes border walls
    // _graphicHandler = _libHandler->makeGraphicLib(_width + 2, _height + 2);
    // _food = std::make_unique<Food>(chooseRandomFoodPos());

    // if (_modeHandler->getIsSound())
    // {
    //     _libHandler->openLib(LibHandler::SOUND, LibHandler::LIBSOUND);
    //     _modeHandler->setSoundHandler(_libHandler->makeSoundLib());
    // }
}

Game::~Game()
{
    LOG_DEBUG("Destructing Game");
}

#include <iostream>
void Game::playTurn()
{
    std::clock_t now = std::clock();
    if (now == (std::clock_t)-1)
        throw std::runtime_error("Error Game::playTurn(): std::clock() failed");

    if (now - _lastTurn > _gameSpeed)
    {
        _lastTurn = now;
        movePlayers();
        // Can probably use the same one for both players in a switch
        collision_type collision_p0 = checkPlayerCollision(_p0);
        if (collision_p0 == COLLISION_P0_FOOD)
        {
            _p0->growBody();
            if (_p0->_body._deque.size() == _totalFreeSpace)
                throw std::runtime_error("Game Win");

            _modeHandler->resetHungerTimer(0, now);
            _food = generateFood();
        }
        else if (collision_p0 == COLLISION_P0_ITSELF || collision_p0 == COLLISION_P0_WALL)
        {
            throw std::runtime_error("Game Over");
        }
        runModesRoutine(now);
        // collision_type collision_p1 = checkPlayerCollision(_p1);
        // Throw error if collision meaning game over
    }
}

void Game::runModesRoutine(const std::clock_t &now)
{
    size_t retMode = _modeHandler->checkRoutine(now);
    // Need to handle hunger, probably throw some exceptions
    if (retMode & 0x4)
    {
        std::cout << "Moving food" << std::endl;
        _food = generateFood();
    }
    if (retMode & 0x8)
    {
        _modeHandler->modifyGameSpeed(_gameSpeed);
    }
}

std::shared_ptr<Player> Game::getP0()
{
    return _p0;
}

// std::shared_ptr<Player> Game::getP1()

std::shared_ptr<Food> Game::getFood()
{
    return _food;
}

void Game::movePlayers() noexcept
{
    if (_p0 != nullptr)
        _p0->move();
    // if (_p1 != nullptr)
    // _p1->move();
}

Game::collision_type Game::checkPlayerCollision(const std::shared_ptr<Player> &player) const noexcept
{
    if (checkCollisionPlayerWall(player))
    {
        return COLLISION_P0_WALL;
    }
    if (checkCollisionPlayerFood(player))
    {
        return COLLISION_P0_FOOD;
    }
    if (checkCollisionPlayerItself(player))
    {
        return COLLISION_P0_ITSELF;
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

// OLD CODE
// Game::Game(std::unique_ptr<ModeHandler> modeHandler)
//     : _modeHandler(std::move(modeHandler)), _width(_modeHandler->getWidth()), _height(_modeHandler->getHeight()),
//       _totalSpace(_width * _height), _gameSpeed(DEFAULT_GAME_SPEED), _turnStart(std::clock())
// {
//     LOG_DEBUG("Constructing Game");
//     _rng.seed(getpid());
//     // _libHandler = std::make_unique<LibHandler>(_width, _height);
//     // +2 to includes border walls
//     // _graphicHandler = _libHandler->makeGraphicLib(_width + 2, _height + 2);
//     // _food = std::make_unique<Food>(chooseRandomFoodPos());

//     // if (_modeHandler->getIsSound())
//     // {
//     //     _libHandler->openLib(LibHandler::SOUND, LibHandler::LIBSOUND);
//     //     _modeHandler->setSoundHandler(_libHandler->makeSoundLib());
//     // }
//     // initPlayer();
// }

// Game::~Game()
// {
//     LOG_DEBUG("Destructing Game");
//     // _libHandler->destroyGraphicLib(std::move(_graphicHandler)); HERE

//     // if (_modeHandler->getIsSound())
//     //     _libHandler->destroySoundLib(_modeHandler->getSoundHandler());
// }

// void Game::initPlayer()
// {
//     if (_modeHandler->getIsSinglePlayer())
//     {
//         _arrayPlayer[0] = std::make_unique<Player>(_width / 2, _height / 2, DEFAULT_PLAYER_SIZE);
//         _arrayPlayer[1] = nullptr;
//         return;
//     }
//     _arrayPlayer[0] = std::make_unique<Player>(1, _height / 2, DEFAULT_PLAYER_SIZE);
//     _arrayPlayer[1] = std::make_unique<Player>(_width - 2, _height / 2, DEFAULT_PLAYER_SIZE);
// }

// void Game::loop(void)
// {
//     clock_t now;
//     while (1)
//     {
//         _graphicHandler->registerPlayerInput();
//         if (_graphicHandler->getPlayerInput(0) == QUIT)
//             break;
//         else if (_graphicHandler->getPlayerInput(0) >= INPUT_SWAP_LIBNCURSES &&
//                  _graphicHandler->getPlayerInput(0) <= INPUT_SWAP_LIBRAYLIB)
//         {
//             if (handleLibSwitch())
//                 break;
//         }

//         now = std::clock();
//         _turn = (now - _turnStart) / (double)CLOCKS_PER_SEC;
//         if (_turn > _gameSpeed)
//         {
//             _arrayPlayer[0]->setDirection(_graphicHandler->getPlayerInput(0));
//             if (handleMultiplayerInput())
//                 break;
//             playersAction(MOVE);
//             checkCollisions();
//             _modeHandler->handleHunger(now, _arrayPlayer[0].get(), _arrayPlayer[1].get());
//             checkPlayerState();
//             if (_modeHandler->getIsSinglePlayer() && _totalSpace <= _arrayPlayer[0]->getPlayerScore())
//                 throwGameOverScore("Game Win");
//             _graphicHandler->clearBoard();
//             playersAction(DRAW);
//             _graphicHandler->drawFood(_food->getPos());
//             _food = _modeHandler->handleDisappearingFood(*this, std::move(_food), now);
//             _modeHandler->serverAction(Server::SEND_DATA, constructGameData());
//             _modeHandler->updateScore(_arrayPlayer[0]->getPlayerScore(), _graphicHandler.get());
//             _turnStart = clock();
//         }
//     }
// }

// void Game::playersAction(player_action_e playerAction)
// {
//     for (auto &&player : _arrayPlayer)
//     {
//         if (player == nullptr)
//             continue;

//         switch (playerAction)
//         {
//         case MOVE:
//             player->move();
//             break;
//         case DRAW:
//             _graphicHandler->drawPlayer(*player);
//             break;
//         case UPDATE_SCORE:
//             _modeHandler->updateScore(player->getPlayerScore(), this->_graphicHandler.get());
//             break;
//         }
//     }
// }

// int Game::handleLibSwitch()
// {
//     // To refactor
//     try
//     {
//         // HERE     // if (_graphicHandler->getPlayerInput(0) == SWAP_LIBNCURSES)
//         //      _graphicHandler = _libHandler->switchGraphicLib(LibHandler::LIBNCURSES,
//         std::move(_graphicHandler));
//         //  else if (_graphicHandler->getPlayerInput(0) == SWAP_LIBSDL)
//         //      _graphicHandler = _libHandler->switchGraphicLib(LibHandler::LIBSDL, std::move(_graphicHandler));
//         //  else if (_graphicHandler->getPlayerInput(0) == SWAP_LIBRAYLIB)
//         //      _graphicHandler = _libHandler->switchGraphicLib((LibHandler::lib_graphic_e)(SWAP_LIBRAYLIB - 3),
//         //                                                      std::move(_graphicHandler));
//         _graphicHandler->resetPlayerInput();
//         _turnStart = clock();
//         return 0;
//     }
//     catch (const std::exception &e)
//     {
//         std::cerr << "In Game: handleLibSwitch(): " << std::endl;
//         return 1;
//     }
// }

// void Game::checkCollisions() const
// {
//     for (auto &&player : _arrayPlayer)
//     {
//         if (player == nullptr)
//             continue;
//         player->setPlayerCollision(STATE_NOTHING);
//         auto playerHeadPoint = player->getHead();
//         if (0 > playerHeadPoint->x || playerHeadPoint->x >= _width || 0 > playerHeadPoint->y ||
//             playerHeadPoint->y >= _height)
//         {
//             player->setPlayerCollision(STATE_WALL_COLL);
//             return;
//         }
//         if (*playerHeadPoint == _food->getPos())
//         {
//             player->setPlayerCollision(STATE_FOOD);
//             continue;
//         }
//         for (auto playerBodyIt = player->getHead() + 1; playerBodyIt != player->getTail(); ++playerBodyIt)
//         {
//             if (*playerBodyIt == *playerHeadPoint)
//             {
//                 player->setPlayerCollision(STATE_BODY_COLL);
//                 return;
//             }
//         }
//         for (auto &&otherPlayer : _arrayPlayer)
//         {
//             if (otherPlayer == nullptr || otherPlayer == player)
//                 continue;
//             for (auto playerBodyIt = otherPlayer->getHead(); playerBodyIt != otherPlayer->getTail();
//             ++playerBodyIt)
//             {
//                 if (*playerBodyIt == *playerHeadPoint)
//                 {
//                     player->setPlayerCollision(STATE_PLAYER_COLL);
//                     return;
//                 }
//             }
//         }
//     }
// }

// void Game::checkPlayerState()
// {
//     for (auto &&player : _arrayPlayer)
//     {
//         if (player == nullptr)
//             continue;
//         switch (player->getPlayerCollision())
//         {
//         case STATE_NOTHING:
//             break;
//         case STATE_WALL_COLL:
//             handleGameOver("Game Over: A Player went in a wall", player->getPlayerIdx());
//             break;
//         case STATE_BODY_COLL:
//             handleGameOver("Game Over: A Player ate itself", player->getPlayerIdx());
//             break;
//         case STATE_FOOD:
//             _modeHandler->playSound(ISoundLib::SOUND_EAT);
//             _modeHandler->changeGameSpeed(SPEED_MODIFIER, *this);
//             player->growBody();
//             player->setHungerTimer(std::clock());
//             _food.reset(new Food(chooseRandomFoodPos()));
//             break;
//         case STATE_PLAYER_COLL:
//             handleGameOver("Game Over: A Player tried to eat the other player", player->getPlayerIdx());
//             break;
//         case STATE_HUNGER:
//             handleGameOver("Game Over: A Player died of starvation", player->getPlayerIdx());
//             break;
//         }
//     }
// }

// point_t Game::chooseRandomFoodPos()
// {
//     point_t point = generateRandomPoint();
//     for (int pass = 0; !isTileFree((point = generateRandomPoint())); pass++)
//     {
//         if (pass == 5000)
//             return {-1, -1};
//     }
//     return point;
// }

// bool Game::isTileFree(point_t point)
// {
//     for (auto &&player : _arrayPlayer)
//     {
//         if (player == nullptr)
//             continue;
//         for (auto &playerPoint : player->getBody())
//             if (playerPoint == point)
//                 return false;
//     }
//     return true;
// }

// point_t Game::generateRandomPoint()
// {
//     std::uniform_int_distribution<int> rngWidth(0, _width - 1);
//     std::uniform_int_distribution<int> rngHeight(0, _height - 1);
//     return {rngWidth(_rng), (rngHeight(_rng))};
// }

// int Game::handleMultiplayerInput()
// {
//     player_input_t clientInput;
//     if (!_modeHandler->getIsSinglePlayer())
//     {
//         if (_modeHandler->getIsMultiLocal())
//             clientInput = _graphicHandler->getPlayerInput(1);
//         if (_modeHandler->getIsMultiNetwork())
//             clientInput = _modeHandler->serverAction(Server::READ_DATA, "", _arrayPlayer[1].get());
//         if (clientInput == QUIT)
//             return -1;
//         _arrayPlayer[1]->setDirection(clientInput);
//     }
//     return 0;
// }

// std::string Game::constructGameData() const
// {
//     std::string str;
//     for (auto &&player : _arrayPlayer)
//     {
//         if (player == nullptr)
//             continue;
//         str.append("p");
//         str.append(std::to_string(player->getPlayerIdx()));
//         str.append("d");
//         if (player->getCurrentDir() > 0)
//             str.append("+");
//         str.append(std::to_string(player->getCurrentDir()));
//         str.append(":");
//         for (auto &playerBody : player->getBody())
//         {
//             if (playerBody.x < 10)
//                 str.append("0");
//             str.append(std::to_string(playerBody.x));
//             str.append(" ");
//             if (playerBody.y < 10)
//                 str.append("0");
//             str.append(std::to_string(playerBody.y));
//             str.append("|");
//         }
//         str.append("\n");
//     }
//     str.append("f:");
//     if (_food->getPos().x < 10)
//         str.append("0");
//     str.append(std::to_string(_food->getPos().x));
//     str.append(" ");
//     if (_food->getPos().y < 10)
//         str.append("0");
//     str.append(std::to_string(_food->getPos().y));
//     str.append("\n");
//     return str;
// }

// void Game::handleGameOver(std::string str, int playerIdx)
// {
//     if (!_modeHandler->getIsSinglePlayer())
//     {
//         std::string playerWinStr("Player ");
//         playerWinStr.append(std::to_string((playerIdx ^ 1)));
//         playerWinStr.append(" wins !");
//         _modeHandler->serverAction(Server::SEND_DATA, std::string("m:" + playerWinStr));
//         throwGameOverScore(str + "\n" + playerWinStr);
//     }
//     throwGameOverScore(str);
// }

// double Game::getGameSpeed() const
// {
//     return _gameSpeed;
// }

// void Game::setGameSpeed(double newSpeed)
// {
//     _gameSpeed = newSpeed;
// }

// void Game::throwGameOverScore(std::string_view str) const
// {
//     throw Game::GameOverException(str, _arrayPlayer[0]->getPlayerScore(), _width, _height,
//                                   _modeHandler->getScoreHandler());
// }

// Game::Game(const Game &other)
// {
//     *this = other;
// }

// Game &Game::operator=(const Game &other)
// {
//     if (&other == this)
//         return *this;
//     _width = other._width;
//     _height = other._height;
//     _totalSpace = other._totalSpace;
//     _rng = other._rng;
//     // _libHandler = std::make_unique<LibHandler>(_width, _height);
//     // _graphicHandler = _libHandler->makeGraphicLib(); //Here
//     _modeHandler = std::make_unique<ModeHandler>();
//     return *this;
// }

// Game::GameOverException::GameOverException(std::string_view str, int playerScore, int width, int height,
//                                            Score *scoreHandler)
// {
//     _msg = str;
//     if (scoreHandler == nullptr)
//         return;
//     _msg += PLAYER_SCORE_STR;
//     _msg += std::to_string(playerScore);
//     _msg += MAP_WIDTH_STR;
//     _msg += std::to_string(width);
//     _msg += MAP_HEIGHT_STR;
//     _msg += std::to_string(height);
// }

// const char *Game::GameOverException::what() const throw()
// {
//     return _msg.c_str();
// }