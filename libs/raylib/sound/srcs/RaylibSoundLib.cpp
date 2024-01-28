#include "../inc/RaylibSoundLib.hpp"
// Not using LOGGER here because I don't want to import LOGGER in libs/
#include <iostream> // std::cerr

RaylibSoundLib::RaylibSoundLib()
{
    SetTraceLogLevel(LOG_ERROR);
    InitAudioDevice();
    _sfxEating = LoadSound("./libs/raylib/sound/assets/eating.wav");
    _muted = false;
#ifdef DEBUG
    std::cerr << "RaylibSoundLib()" << std::endl;
#endif
}

RaylibSoundLib::~RaylibSoundLib()
{
    UnloadSound(_sfxEating);
    CloseAudioDevice();
#ifdef DEBUG
    std::cerr << "~RaylibSoundLib()" << std::endl;
#endif
}

void RaylibSoundLib::playSound(sound_type_e soundType) const
{
    if (_muted)
        return;
    switch (soundType)
    {
    case ISoundLib::SOUND_EAT:
        PlaySound(_sfxEating);
        break;
    }
}

void RaylibSoundLib::inverseMute()
{
    _muted = !_muted;
    std::cout << (_muted ? "Muted" : "Unmuted") << std::endl;
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