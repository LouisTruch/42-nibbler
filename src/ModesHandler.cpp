#include "../inc/Game.hpp"
#include "../inc/Menu/MenuItem.hpp"
#include "../inc/ModeHandler.hpp"
#include <cmath>
#include <ctime>
#include <iostream>
#include <memory>

// This constructor is for debugging purpose
ModeHandler::ModeHandler()
    : _width(20), _height(20), _isChangingSpeed(false), _isDisappearingFood(false), _isHunger(false),
      _scoreHandler(nullptr), _isSound(false), _soundHandler(nullptr), _isSinglePlayer(false), _isMultiLocal(false),
      _isMultiNetwork(true)
{
    if (_isMultiNetwork)
    {
        try
        {
            _server = std::make_unique<Server>();
            serverAction(Server::WAIT_CONNECTION);
            serverAction(Server::SEND_DATA, constructGameInitData());
        }
        catch (const std::exception &e)
        {
            std::cerr << "In ModeHandler():" << e.what() << std::endl;
            _server = nullptr;
        }
    }
}

ModeHandler::ModeHandler(int_gameConfig_t config, board_size_t boardSize) : _width(boardSize.x), _height(boardSize.y)
{
    _isChangingSpeed = config & (int)(std::pow(2, (int)MenuItem::CHANGING_SPEED)) ? true : false;
    _isDisappearingFood = config & (int)(std::pow(2, (int)MenuItem::DISAPPEARING_FOOD)) ? true : false;
    if (_isDisappearingFood)
        _foodTimer = std::clock();
    _isHunger = config & (int)(std::pow(2, (int)MenuItem::HUNGER)) ? true : false;
    if (_isHunger)
        _hungerTimer = std::clock();
    _isSinglePlayer = config & (int)(std::pow(2, (int)MenuItem::SINGLE_PLAYER)) ? true : false;
    if (!_isSinglePlayer)
    {
        if (config & (int)(std::pow(2, (int)MenuItem::SCORE)))
            std::cout << "Score is disabled in multiplayer\n";
        _scoreHandler = nullptr;
    }
    else
        _scoreHandler =
            config & (int)(std::pow(2, (int)MenuItem::SCORE)) ? std::make_unique<Score>(_width, _height) : nullptr;
    _isMultiLocal = config & (int)(std::pow(2, (int)MenuItem::MULTI_LOCAL)) ? true : false;
    _isSound = config & (int)(std::pow(2, (int)MenuItem::SOUND)) ? true : false;
    _soundHandler = nullptr;
    _isMultiNetwork = config & (int)(std::pow(2, (int)MenuItem::MULTI_NETWORK)) ? true : false;
    if (config & (int)(std::pow(2, (int)MenuItem::MULTI_NETWORK)))
        instantiateServer();
    else
        _server = nullptr;
}

void ModeHandler::instantiateServer()
{
    try
    {
        _server = std::make_unique<Server>();
        serverAction(Server::WAIT_CONNECTION);
        serverAction(Server::SEND_DATA, constructGameInitData());
    }
    catch (const std::exception &e)
    {
        std::cerr << "In ModeHandler():" << e.what() << std::endl;
        _server = nullptr;
    }
}

ModeHandler::~ModeHandler()
{
}

void ModeHandler::changeGameSpeed(double coeff, Game &game)
{
    if (!_isChangingSpeed)
        return;
    if (game.getGameSpeed() >= MAX_SPEED)
        return;
    game.setGameSpeed(game.getGameSpeed() * coeff);
}

std::unique_ptr<Food> ModeHandler::handleDisappearingFood(Game &game, std::unique_ptr<Food> food, clock_t now)
{
    if (!_isDisappearingFood || !(((now - _foodTimer) / (double)CLOCKS_PER_SEC) >= FOOD_TIMER))
        return food;
    _foodTimer = std::clock();
    return std::make_unique<Food>(game.chooseRandomFoodPos());
}

void ModeHandler::handleHunger(clock_t now, Player *player0, Player *player1)
{
    if (!_isHunger)
        return;

    if (player0)
        if (((now - player0->getHungerTimer()) / (double)CLOCKS_PER_SEC) >= HUNGER_DEATH_TIMER)
            player0->setPlayerCollision(STATE_HUNGER);
    if (player1)
        if (((now - player1->getHungerTimer()) / (double)CLOCKS_PER_SEC) >= HUNGER_DEATH_TIMER)
            player1->setPlayerCollision(STATE_HUNGER);
}

void ModeHandler::resetHungerTimer(clock_t now)
{
    if (_isHunger)
        _hungerTimer = now;
}

void ModeHandler::updateScore(int score, IGraphicLib *graphicHandler)
{
    if (_scoreHandler == nullptr || graphicHandler == NULL)
        return;
    _scoreHandler->setCurrentScore(score);
    if (_scoreHandler->getCurrentScore() > _scoreHandler->getHighScoreFile())
        graphicHandler->drawScores(_scoreHandler->getCurrentScore(), _scoreHandler->getCurrentScore());
    else
        graphicHandler->drawScores(_scoreHandler->getCurrentScore(), _scoreHandler->getHighScoreFile());
}

void ModeHandler::playSound(ISoundLib::sound_type_e sound) const
{
    if (!_isSound)
        return;
    _soundHandler->playSound(sound);
}

player_input_t ModeHandler::serverAction(Server::server_action_e action, std::string data, Player *player)
{
    if (_server == nullptr)
        return DEFAULT;
    switch (action)
    {
    case Server::WAIT_CONNECTION:
        _server->waitConnection();
        break;
    case Server::SEND_DATA:
        _server->sendData(data);
        break;
    case Server::READ_DATA:
        if (player != nullptr)
        {
            try
            {
                player_input_t clientInput = (player_input_t)_server->readData();
                if (clientInput != DEFAULT)
                    return clientInput;
            }
            catch (const std::exception &e)
            {
                std::cerr << "In ModeHandler::serverAction(): " << e.what() << std::endl;
                std::cerr << "Client probably rqed..." << std::endl;
                return QUIT;
            }
        }
        break;
    }
    return DEFAULT;
}

const std::string ModeHandler::constructGameInitData()
{
    std::string buffer;
    buffer.append("i:");
    buffer += std::to_string(_width);
    buffer.append(" ");
    buffer += std::to_string(_height);
    buffer.append(" ");
    buffer += _isSound ? '1' : '0';
    return buffer;
}

void ModeHandler::setSoundHandler(std::unique_ptr<ISoundLib> sLib)
{
    _soundHandler = std::move(sLib);
}

bool ModeHandler::getIsSound() const
{
    return _isSound;
}

std::unique_ptr<ISoundLib> ModeHandler::getSoundHandler()
{
    return std::move(_soundHandler);
}

int ModeHandler::getWidth() const
{
    return _width;
}

int ModeHandler::getHeight() const
{
    return _height;
}

bool ModeHandler::getIsSinglePlayer() const
{
    return _isSinglePlayer;
}

bool ModeHandler::getIsMultiLocal() const
{
    return _isMultiLocal;
}

bool ModeHandler::getIsMultiNetwork() const
{
    return _isMultiNetwork;
}

Score *ModeHandler::getScoreHandler() const
{
    return _scoreHandler.get();
}