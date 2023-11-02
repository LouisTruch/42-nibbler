#pragma once

#include "../../../../inc/Game.hpp"
#include "../../../../inc/ISoundLib.hpp"
#include "../../raylib/src/raylib.h"
#include <memory>

class RaylibSoundLib : public ISoundLib
{
  public:
    RaylibSoundLib(const Game &);
    ~RaylibSoundLib();
    void playSound(sound_type_e) const;

  private:
    Sound _sfxEating;
};

extern "C"
{
    std::unique_ptr<RaylibSoundLib> makeSoundLib(const Game &game);
    void destroySoundLib(std::unique_ptr<RaylibSoundLib> sLib);
}