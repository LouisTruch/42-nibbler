#pragma once

#include "../../../../inc/Interface/ISoundLib.hpp"

class DebugSoundLib : public ISoundLib
{
  public:
    DebugSoundLib();
    ~DebugSoundLib();
    void playSound(sound_type_e) const;
    void inverseMute();

  private:
    DebugSoundLib(const DebugSoundLib &) = delete;
    DebugSoundLib &operator=(const DebugSoundLib &) = delete;
};

extern "C"
{
    DebugSoundLib *makeSoundLib();
    void destroySoundLib(DebugSoundLib *sLib);
}