#include "../inc/Game.hpp"
#include "../inc/Menu/MenuItem.hpp"
#include "../inc/ModeHandler.hpp"
#include <cmath>
#include <ctime>
#include <iostream>
#include <memory>

ModeHandler::ModeHandler()
    : _width(10), _height(10), _isChangingSpeed(false), _isDisappearingFood(false), _isHunger(false), _isScore(false),
      _scoreHandler(nullptr), _isSound(false), _soundHandler(nullptr)
{
}

ModeHandler::ModeHandler(int_gameConfig_t config, int width, int height) : _width(width), _height(height)
{
    _isChangingSpeed = config & (int)(std::pow(2, (int)MenuItem::CHANGING_SPEED)) ? true : false;
    _isDisappearingFood = config & (int)(std::pow(2, (int)MenuItem::DISAPPEARING_FOOD)) ? true : false;
    if (_isDisappearingFood)
        _foodTimer = std::clock();
    _isHunger = config & (int)(std::pow(2, (int)MenuItem::HUNGER)) ? true : false;
    if (_isHunger)
        _hungerTimer = std::clock();
    _scoreHandler =
        config & (int)(std::pow(2, (int)MenuItem::SCORE)) ? std::make_unique<Score>(width, height) : nullptr;
    // if (_isScore)
    //     _scoreHandler = std::make_unique<Score>(width, height);
    // else
    //     _scoreHandler = nullptr;
    // _isChangingSpeed = conf(int)(std::pow(2, (int)MenuItem::MULTI_OFF + 1) ? true : false;
    // _isChangingSpeed = conf(int)(std::pow(2, (int)MenuItem::MULTI_LOCAL + 1) ? true : false;
    // _isChangingSpeed = conf(int)(std::pow(2, (int)MenuItem::MULTI_NETWORK + 1) ? true : false;
    _isSound = config & (int)(std::pow(2, (int)MenuItem::SOUND)) ? true : false;
    _soundHandler = nullptr;
}

ModeHandler::~ModeHandler()
{
}

ModeHandler::ModeHandler(const ModeHandler &other)
{
    *this = other;
}

ModeHandler &ModeHandler::operator=(const ModeHandler &other)
{
    if (&other == this)
        return *this;
    _isChangingSpeed = other._isChangingSpeed;
    _isDisappearingFood = other._isDisappearingFood;
    _foodTimer = other._foodTimer;
    _isSound = other._isSound;
    _soundHandler = nullptr;
    return *this;
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

bool ModeHandler::handleHunger(clock_t now)
{
    return (_isHunger && (((now - _hungerTimer) / (double)CLOCKS_PER_SEC) >= HUNGER_TIMER));
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