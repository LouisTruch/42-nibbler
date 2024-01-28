#pragma once

#include "../../../../inc/Game.hpp"
#include "../../../../inc/Interface/ISoundLib.hpp"
#include "../../raylib/src/raylib.h"

class RaylibSoundLib : public ISoundLib
{
  public:
    RaylibSoundLib();
    ~RaylibSoundLib();
    void playSound(sound_type_e) const;

  private:
    RaylibSoundLib(const RaylibSoundLib &) = delete;
    RaylibSoundLib &operator=(const RaylibSoundLib &) = delete;

  private:
    Sound _sfxEating;
};

extern "C"
{
    RaylibSoundLib *makeSoundLib();
    void destroySoundLib(RaylibSoundLib *sLib);
}