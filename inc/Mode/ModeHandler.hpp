#pragma once

#include "../types.hpp"
#include "ModeAcceleratingSpeed.hpp"
#include "ModeHunger.hpp"
#include "ModeMovingFood.hpp"
#include <chrono> // std::chrono
#include <ctime>  // std::clock_t
#include <memory> // std::unique_ptr
#include <string> // std::string

class ModeHandler
{
  public:
#ifdef DEBUG
    ModeHandler();
#endif
    ModeHandler(int_GameConfig_t);
    ~ModeHandler();
    ModeHandler(const ModeHandler &) = delete;
    ModeHandler &operator=(const ModeHandler &) = delete;

  public:
    typedef enum
    {
        NOT_ACTIVE = -1,
        ACTIVE = 0,
        CONDITION_MET,
    } ModeCurrentState_e;
    typedef struct
    {
        ModeCurrentState_e hungerP0;
        ModeCurrentState_e hungerP1;
        ModeCurrentState_e movingFood;
        ModeCurrentState_e acceleratingSpeed;
    } ModeState_t;

  public:
    void doCheckRoutine(const std::chrono::time_point<std::chrono::high_resolution_clock> &now);
    void resetHungerTimer(int playerIdx,
                          const std::chrono::time_point<std::chrono::high_resolution_clock> &now) noexcept;
    void modifyGameSpeed(long int &) noexcept;
    ModeCurrentState_e getHungerState(int playerIdx) const noexcept;
    ModeCurrentState_e getMovingFoodState() const noexcept;
    ModeCurrentState_e getAcceleratingSpeedState() const noexcept;
    void resetModeStates() noexcept;

  private:
    std::string getInfo() const;

  private:
    ModeState_t _modeState;
    std::unique_ptr<ModeHunger> _hungerHandler;
    std::unique_ptr<ModeMovingFood> _movingFoodHandler;
    std::unique_ptr<ModeAcceleratingSpeed> _acceleratingSpeedHandler;
};