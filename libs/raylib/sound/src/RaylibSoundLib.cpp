#include "../inc/RaylibSoundLib.hpp"

RaylibSoundLib::RaylibSoundLib(const Game &game)
{
    SetTraceLogLevel(LOG_ERROR);
    InitAudioDevice();
    _sfxEating = LoadSound("./libs/raylib/sound/assets/eating.wav");
    (void)game;
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

std::unique_ptr<RaylibSoundLib> makeSoundLib(const Game &game)
{
    return std::make_unique<RaylibSoundLib>(game);
}

void destroySoundLib(std::unique_ptr<RaylibSoundLib> sLib)
{
    sLib.reset();
}