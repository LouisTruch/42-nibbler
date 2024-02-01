#include "../inc/DebugSoundLib.hpp"

#include <iostream> // std::cout
DebugSoundLib::DebugSoundLib()
{
    std::cout << "DebugSoundLib()" << std::endl;
}

DebugSoundLib::~DebugSoundLib()
{
    std::cout << "~DebugSoundLib()" << std::endl;
}

void DebugSoundLib::playSound(sound_type_e soundType) const
{
    if (_muted)
    {
        std::cout << "DebugSoundLib::playSound(" << soundType << ") muted" << std::endl;
        return;
    }
    std::cout << "DebugSoundLib::playSound(" << soundType << ")" << std::endl;
}

void DebugSoundLib::inverseMute()
{
    _muted = !_muted;
    std::cout << (_muted ? "Muted" : "Unmuted") << std::endl;
}

extern "C"
{
    DebugSoundLib *makeSoundLib()
    {
        return new DebugSoundLib();
    }

    void destroySoundLib(DebugSoundLib *sLib)
    {
        delete sLib;
    }
}