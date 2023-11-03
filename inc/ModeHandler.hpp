#pragma once

#include "Food.hpp"
#include "IGraphicLib.hpp"
#include "ISoundLib.hpp"
#include "Score.hpp"
#include "types.hpp"
#include <ctime>
#include <memory>

class Game;

constexpr double SPEED_MODIFIER = 0.95;
constexpr double MAX_SPEED = 0.8;
constexpr double FOOD_TIMER = 4;
constexpr double HUNGER_TIMER = 10;

class ModeHandler
{
  public:
    ModeHandler();
    ModeHandler(int_gameConfig_t, int, int);
    ~ModeHandler();
    ModeHandler(const ModeHandler &);
    ModeHandler &operator=(const ModeHandler &);
    void changeGameSpeed(double, Game &);
    std::unique_ptr<Food> handleDisappearingFood(Game &, std::unique_ptr<Food>, clock_t);
    bool handleHunger(clock_t);
    void resetHungerTimer(clock_t);
    bool getIsSound() const;
    void setSoundHandler(std::unique_ptr<ISoundLib>);
    void playSound(ISoundLib::sound_type_e) const;
    void updateScore(int, IGraphicLib *);
    std::unique_ptr<ISoundLib> getSoundHandler();
    int getWidth() const;
    int getHeight() const;

  private:
    int _width;
    int _height;
    bool _isChangingSpeed;
    bool _isDisappearingFood;
    bool _isHunger;
    std::clock_t _hungerTimer;
    bool _isScore;
    std::unique_ptr<Score> _scoreHandler;
    bool _isSound;
    std::clock_t _foodTimer;
    std::unique_ptr<ISoundLib> _soundHandler;
};