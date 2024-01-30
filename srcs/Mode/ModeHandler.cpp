#include "../../inc/Mode/ModeHandler.hpp"
#include "../../inc/Log/Logger.hpp"

// This constructor is for debugging purpose
ModeHandler::ModeHandler() : _hungerHandler(nullptr), _movingFoodHandler(nullptr), _acceleratingSpeedHandler(nullptr)
{
}

#include <memory> // std::make_unique
ModeHandler::ModeHandler(int_gameConfig_t config)
{
    // New + Have to check config
    // _hungerHandler = nullptr;
    _movingFoodHandler = nullptr;
    _acceleratingSpeedHandler = nullptr;
    // _acceleratingSpeedHandler = std::make_unique<ModeAcceleratingSpeed>();
    // _movingFoodHandler = std::make_unique<ModeMovingFood>();
    _hungerHandler = std::make_unique<ModeHunger>();
    LOG_DEBUG("Constructing ModeHandler " + this->getInfo());
}

ModeHandler::~ModeHandler()
{
    LOG_DEBUG("Destructing ModeHandler");
}

size_t ModeHandler::checkRoutine(const std::chrono::time_point<std::chrono::high_resolution_clock> &now)
{
    size_t ret = 0;
    // TODO : change type I think
    if (_hungerHandler != nullptr)
    {
        int hungerRet = _hungerHandler->check(now);
        if (hungerRet & 0x1)
        {
            ret |= 0x1;
        }
        if (hungerRet & 0x2)
        {
            ret |= 0x2;
        }
    }
    if (_movingFoodHandler != nullptr)
    {
        ret |= (_movingFoodHandler->check(now) * 0x4);
    }
    if (_acceleratingSpeedHandler != nullptr)
    {
        ret |= (_acceleratingSpeedHandler->check(now) * 0x8);
    }
    LOG_DEBUG("ret: " << std::to_string(ret));
    return ret;
}

void ModeHandler::resetHungerTimer(int playerIndex,
                                   const std::chrono::time_point<std::chrono::high_resolution_clock> &now) noexcept
{
    if (_hungerHandler != nullptr)
    {
        _hungerHandler->resetHungerTimer(playerIndex, now);
    }
}

void ModeHandler::modifyGameSpeed(long int &gameSpeed) noexcept
{
    if (_acceleratingSpeedHandler != nullptr)
    {
        _acceleratingSpeedHandler->modifyGameSpeed(gameSpeed);
    }
}

std::string ModeHandler::getInfo() const
{
    std::string info = "Info:";
    info += " Hunger:";
    info += (_hungerHandler != nullptr) ? "1" : "0";
    info += " MovingFood:";
    info += (_movingFoodHandler != nullptr) ? "1" : "0";
    info += " AcceleratingSpeed:";
    info += (_acceleratingSpeedHandler != nullptr) ? "1" : "0";
    return info;
    // return "ModeHandler: " + std::to_string(_width) + "x" + std::to_string(_height) +
    //        " ChangingSpeed:" + std::to_string(_isChangingSpeed) +
    //        " DisappearingFood:" + std::to_string(_isDisappearingFood) + " Hunger:" + std::to_string(_isHunger) +
    //        " Sound:" + std::to_string(_isSound) + " SinglePlayer:" + std::to_string(_isSinglePlayer) +
    //        " Local:" + std::to_string(_isMultiLocal) + " Network:" + std::to_string(_isMultiNetwork);
}

// void ModeHandler::instantiateServer()
// {
//     try
//     {
//         _server = std::make_unique<Server>();
//         serverAction(Server::WAIT_CONNECTION);
//         serverAction(Server::SEND_DATA, constructGameInitData());
//     }
//     catch (const std::exception &e)
//     {
//         std::cerr << "In ModeHandler():" << e.what() << std::endl;
//         _server = nullptr;
//     }
// }

// void ModeHandler::changeGameSpeed(double modifier, Game &game)
// {
//     if (!_isChangingSpeed)
//         return;
//     // if (game.getGameSpeed() <= MAX_SPEED)
//     // return;
//     // game.setGameSpeed(game.getGameSpeed() * modifier);
// }

// std::unique_ptr<Food> ModeHandler::handleDisappearingFood(Game &game, std::unique_ptr<Food> food, clock_t now)
// {
//     return food;
//     if (!_isDisappearingFood || !(((now - _foodTimer) / (double)CLOCKS_PER_SEC) >= FOOD_TIMER))
//         return food;
//     // _foodTimer = std::clock();
//     // return std::make_unique<Food>(game.chooseRandomFoodPos());
// }

// void ModeHandler::handleHunger(clock_t now, Player *player0, Player *player1)
// {
//     if (!_isHunger)
//         return;

//     // if (player0)
//     //     if (((now - player0->getHungerTimer()) / (double)CLOCKS_PER_SEC) >= HUNGER_DEATH_TIMER)
//     //         player0->setPlayerCollision(STATE_HUNGER);
//     // if (player1)
//     //     if (((now - player1->getHungerTimer()) / (double)CLOCKS_PER_SEC) >= HUNGER_DEATH_TIMER)
//     //         player1->setPlayerCollision(STATE_HUNGER);
// }

// void ModeHandler::resetHungerTimer(clock_t now)
// {
//     if (_isHunger)
//         _hungerTimer = now;
// }

// void ModeHandler::updateScore(int score, IGraphicLib *graphicHandler)
// {
//     if (_scoreHandler == nullptr || graphicHandler == NULL)
//         return;
//     _scoreHandler->setCurrentScore(score);
//     if (_scoreHandler->getCurrentScore() > _scoreHandler->getHighScoreFile())
//         graphicHandler->drawScores(_scoreHandler->getCurrentScore(), _scoreHandler->getCurrentScore());
//     else
//         graphicHandler->drawScores(_scoreHandler->getCurrentScore(), _scoreHandler->getHighScoreFile());
// }

// void ModeHandler::playSound(ISoundLib::sound_type_e sound) const
// {
//     if (!_isSound)
//         return;
//     _soundHandler->playSound(sound);
// }

// player_input_t ModeHandler::serverAction(Server::server_action_e action, std::string data, Player *player)
// {
//     if (_server == nullptr)
//         return INPUT_DEFAULT;
//     switch (action)
//     {
//     case Server::WAIT_CONNECTION:
//         _server->waitConnection();
//         break;
//     case Server::SEND_DATA:
//         _server->sendData(data);
//         break;
//     case Server::READ_DATA:
//         if (player != nullptr)
//         {
//             try
//             {
//                 player_input_t clientInput = (player_input_t)_server->readData();
//                 if (clientInput != INPUT_DEFAULT)
//                     return clientInput;
//             }
//             catch (const std::exception &e)
//             {
//                 std::cerr << "In ModeHandler::serverAction(): " << e.what() << std::endl;
//                 std::cerr << "Client probably rqed..." << std::endl;
//                 return QUIT;
//             }
//         }
//         break;
//     }
//     return INPUT_DEFAULT;
// }

// const std::string ModeHandler::constructGameInitData()
// {
//     std::string buffer;
//     buffer.append("i:");
//     buffer += std::to_string(_width);
//     buffer.append(" ");
//     buffer += std::to_string(_height);
//     buffer.append(" ");
//     buffer += _isSound ? '1' : '0';
//     return buffer;
// }

// void ModeHandler::setSoundHandler(std::unique_ptr<ISoundLib> sLib)
// {
//     _soundHandler = std::move(sLib);
// }

// bool ModeHandler::getIsSound() const
// {
//     return _isSound;
// }

// std::unique_ptr<ISoundLib> ModeHandler::getSoundHandler()
// {
//     return std::move(_soundHandler);
// }

// int ModeHandler::getWidth() const
// {
//     return _width;
// }

// int ModeHandler::getHeight() const
// {
//     return _height;
// }

// bool ModeHandler::getIsSinglePlayer() const
// {
//     return _isSinglePlayer;
// }

// bool ModeHandler::getIsMultiLocal() const
// {
//     return _isMultiLocal;
// }

// bool ModeHandler::getIsMultiNetwork() const
// {
//     return _isMultiNetwork;
// }

// Score *ModeHandler::getScoreHandler() const
// {
//     return _scoreHandler.get();
// }