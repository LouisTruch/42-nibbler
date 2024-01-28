#include "../inc/RaylibSoundLib.hpp"

RaylibSoundLib::RaylibSoundLib()
{
    SetTraceLogLevel(LOG_ERROR);
    InitAudioDevice();
    _sfxEating = LoadSound("./libs/raylib/sound/assets/eating.wav");
}

RaylibSoundLib::~RaylibSoundLib()
{
    UnloadSound(_sfxEating);
    CloseAudioDevice();
}

void RaylibSoundLib::playSound(sound_type_e soundType) const
{
    switch (soundType)
    {
    case ISoundLib::SOUND_EAT:
        PlaySound(_sfxEating);
        break;
    }
}

extern "C"
{
    RaylibSoundLib *makeSoundLib()
    {
        return new RaylibSoundLib();
    }

    void destroySoundLib(RaylibSoundLib *sLib)
    {
        delete sLib;
    }
}