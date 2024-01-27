#pragma once

#include "../Food.hpp"
#include "../types.hpp"
#include "ModeAcceleratingSpeed.hpp"
#include "ModeHunger.hpp"
#include "ModeMovingFood.hpp"
#include <ctime>  // std::clock_t
#include <memory> // std::unique_ptr
#include <string> // std::string

class ModeHandler
{
  public:
    ModeHandler();
    ModeHandler(int_gameConfig_t);
    ~ModeHandler();

  public:
    size_t checkRoutine(const std::clock_t &now);
    void resetHungerTimer(int playerIdx, const std::clock_t &now) noexcept;
    void modifyGameSpeed(size_t &) noexcept;
    // Old
    // void changeGameSpeed(double, Game &);
    // std::unique_ptr<Food> handleDisappearingFood(Game &, std::unique_ptr<Food>, clock_t);
    // void handleHunger(clock_t, Player *, Player *);
    // void resetHungerTimer(clock_t);
    // bool getIsSound() const;
    // void setSoundHandler(std::unique_ptr<ISoundLib>);
    // void playSound(ISoundLib::sound_type_e) const;
    // void updateScore(int, IGraphicLib *);
    // player_input_t serverAction(Server::server_action_e, std::string = "", Player * = nullptr);
    // std::unique_ptr<ISoundLib> getSoundHandler();
    // int getWidth() const;
    // int getHeight() const;
    // bool getIsSinglePlayer() const;
    // bool getIsMultiLocal() const;
    // bool getIsMultiNetwork() const;
    // void instantiateServer();
    // const std::string constructGameInitData();
    // Score *getScoreHandler() const;

  private:
    ModeHandler(const ModeHandler &) = delete;
    ModeHandler &operator=(const ModeHandler &) = delete;

  private:
    std::string getInfo() const;

  private:
    // New
    std::unique_ptr<ModeHunger> _hungerHandler;
    std::unique_ptr<ModeMovingFood> _movingFoodHandler;
    std::unique_ptr<ModeAcceleratingSpeed> _acceleratingSpeedHandler;

    // Old
    // bool _isChangingSpeed;
    // bool _isDisappearingFood;
    // bool _isHunger;
    // std::clock_t _hungerTimer;
    // std::unique_ptr<Score> _scoreHandler;
    // bool _isSound;
    // std::clock_t _foodTimer;
    // std::unique_ptr<ISoundLib> _soundHandler;
    // bool _isSinglePlayer;
    // bool _isMultiLocal;
    // bool _isMultiNetwork;
    // std::unique_ptr<Server> _server;
};