#pragma once

class ISoundLib
{
  public:
    typedef enum sound_type
    {
        SOUND_EAT,
    } sound_type_e;

  public:
    virtual ~ISoundLib() = default;
    virtual void playSound(sound_type_e) const = 0;
};