#pragma once

#include "Food.hpp"
#include "IGraphicLib.hpp"
#include "ISoundLib.hpp"
#include "Score.hpp"
#include "Server.hpp"
#include "types.hpp"
#include <array>
#include <ctime>
#include <memory>

class Game;

constexpr double SPEED_MODIFIER = 0.95;
constexpr double MAX_SPEED = 0.8;
constexpr double FOOD_TIMER = 4;
constexpr double HUNGER_DEATH_TIMER = 10;

class ModeHandler
{
  public:
    ModeHandler();
    ModeHandler(int_gameConfig_t, board_size_t);
    ~ModeHandler();
    void changeGameSpeed(double, Game &);
    std::unique_ptr<Food> handleDisappearingFood(Game &, std::unique_ptr<Food>, clock_t);
    void handleHunger(clock_t, Player *, Player *);
    void resetHungerTimer(clock_t);
    bool getIsSound() const;
    void setSoundHandler(std::unique_ptr<ISoundLib>);
    void playSound(ISoundLib::sound_type_e) const;
    void updateScore(int, IGraphicLib *);
    player_input_t serverAction(Server::server_action_e, std::string = "", Player * = nullptr);
    std::unique_ptr<ISoundLib> getSoundHandler();
    int getWidth() const;
    int getHeight() const;
    bool getIsSinglePlayer() const;
    bool getIsMultiLocal() const;
    bool getIsMultiNetwork() const;
    void instantiateServer();
    const std::string constructGameInitData();
    Score *getScoreHandler() const;

  private:
    ModeHandler(const ModeHandler &) = delete;
    ModeHandler &operator=(const ModeHandler &) = delete;

  private:
    int _width;
    int _height;
    bool _isChangingSpeed;
    bool _isDisappearingFood;
    bool _isHunger;
    std::clock_t _hungerTimer;
    std::unique_ptr<Score> _scoreHandler;
    bool _isSound;
    std::clock_t _foodTimer;
    std::unique_ptr<ISoundLib> _soundHandler;
    bool _isSinglePlayer;
    bool _isMultiLocal;
    bool _isMultiNetwork;
    std::unique_ptr<Server> _server;
};