#include "../inc/LibHandler.hpp"
#include "../inc/Log/Logger.hpp"
#include <dlfcn.h>   // dlopen(), dlsym(), dlclose()
#include <iostream>  // std::cerr
#include <stdexcept> // std::runtime_error

LibHandler::LibHandler(board_size_t boardSize)
    : _boardSize(boardSize), _currentGraphicLib(NO_LIB), _graphicLibPtr(nullptr), _makerGraphicFunc(nullptr),
      _deleterGraphicFunc(nullptr), _currentSoundLib(NO_SOUND), _soundLibPtr(nullptr), _makerSoundFunc(nullptr),
      _deleterSoundFunc(nullptr)
{
#ifndef DEBUG
    openGraphicLib(LIBSDL);
#else
    openGraphicLib(LIBDEBUG);
#endif
    loadSymbolsGraphicLib();

    // try
    // {
    //     // TODO : Fix leak coming from here (from dlopen())
    //     openSoundLib(SOUNDRAYLIB);
    //     loadSymbolsSoundLib();
    // }
    // catch (const std::exception &e)
    // {
    //     std::cerr << "In LibHandler(): " << e.what() << std::endl;
    //     std::cerr << "Sound will not be enabled" << std::endl;
    // }
}

LibHandler::~LibHandler()
{
    closeCurrentGraphicLib();
    closeCurrentSoundLib();
    LOG_DEBUG("Destructing LibHandler");
}

void LibHandler::openGraphicLib(lib_graphic_e libChoice)
{
    if (0 < libChoice && libChoice >= NB_GRAPHIC_LIBS)
        return;
    dlerror();
    _graphicLibPtr = dlopen(GRAPHIC_LIB_PATH[libChoice].data(), RTLD_LAZY | RTLD_LOCAL);
    if (!_graphicLibPtr)
    {
        std::cerr << "Error LibHandler(): " << dlerror() << std::endl;
        throw std::runtime_error("Error LibHandler(): could not load .so object");
    }

    _currentGraphicLib = libChoice;

    std::string info;
    switch (libChoice)
    {
    case LIBNCURSES:
        info = "NCURSES";
        break;
    case LIBSDL:
        info = "SDL";
        break;
    case LIBRAYLIB:
        info = "RAYLIB";
        break;
#ifdef DEBUG
    case LIBDEBUG:
        info = "DEBUG";
        break;
#endif
    default:
        info = "NO_LIB";
        break;
    }
    LOG_DEBUG("Successfully loaded " + info + ".so object");
}

void LibHandler::closeCurrentGraphicLib()
{
    if (!_graphicLibPtr)
        return;
    dlerror();
    if (dlclose(_graphicLibPtr))
    {
        std::cerr << "Error LibHandler->closeLib(): " << dlerror() << std::endl;
        throw std::runtime_error("Error LibHandler->closeLib(): Couldnt close lib");
    }
    _graphicLibPtr = nullptr;
    _currentGraphicLib = NO_LIB;
}

std::unique_ptr<IGraphicLib> LibHandler::switchGraphicLib(lib_graphic_e libChoice, std::unique_ptr<IGraphicLib> gLib)
{
    LOG_DEBUG("Switching graphic lib to " + std::string(GRAPHIC_LIB_PATH[libChoice].data()) + ".so");
#ifndef DEBUG
    if (libChoice == _currentGraphicLib)
        return gLib;
#endif

    if (0 < libChoice && libChoice >= NB_GRAPHIC_LIBS)
        return gLib;

    gLib.reset();
    // Probably not needed
    _deleterGraphicFunc(gLib.get());
    closeCurrentGraphicLib();
    openGraphicLib(libChoice);
    loadSymbolsGraphicLib();
    return makeGraphicLib();
}

std::unique_ptr<IGraphicLib> LibHandler::makeGraphicLib(void)
{
    return std::unique_ptr<IGraphicLib>(_makerGraphicFunc(_boardSize.x, _boardSize.y));
}

void LibHandler::destroyGraphicLib(IGraphicLib *gLib)
{
    _deleterGraphicFunc(gLib);
}

void LibHandler::loadSymbolsGraphicLib(void)
{
    _makerGraphicFunc = (makeGraphicLibFunc)dlsym(_graphicLibPtr, "makeGraphicLib");
    _deleterGraphicFunc = (destroyGraphicLibFunc)dlsym(_graphicLibPtr, "destroyGraphicLib");
    if (!_makerGraphicFunc || !_deleterGraphicFunc)
    {
        closeCurrentGraphicLib();
        throw std::runtime_error("Error LibHandler loadSymbols: couldnt load graphic lib functions");
    }
    LOG_DEBUG("Successfully loaded graphic symbols");
}

void LibHandler::openSoundLib(lib_sound_e libChoice)
{
    if (0 < libChoice && libChoice >= NB_SOUND_LIBS)
        return;

    dlerror();
    _soundLibPtr = dlopen(SOUND_LIB_PATH[libChoice].data(), RTLD_LAZY | RTLD_LOCAL);
    if (!_soundLibPtr)
    {
        std::cerr << "Error LibHandler(): " << dlerror() << std::endl;
        throw std::runtime_error("Error LibHandler(): couldnt load .so object");
    }
    _currentSoundLib = libChoice;
    LOG_DEBUG("Successfully loaded SOUND .so object");
}

void LibHandler::closeCurrentSoundLib()
{
    if (!_soundLibPtr)
        return;
    dlerror();
    if (dlclose(_soundLibPtr))
    {
        std::cerr << "Error LibHandler->closeLib(): " << dlerror() << std::endl;
        throw std::runtime_error("Error LibHandler->closeLib(): Couldnt close lib");
    }
    _soundLibPtr = nullptr;
    _currentSoundLib = NO_SOUND;
}

void LibHandler::loadSymbolsSoundLib(void)
{
    dlerror();
    _makerSoundFunc = (makeSoundLibFunc)dlsym(_soundLibPtr, "makeSoundLib");
    _deleterSoundFunc = (destroySoundLibFunc)dlsym(_soundLibPtr, "destroySoundLib");
    if (!_makerSoundFunc || !_deleterSoundFunc)
    {
        closeCurrentSoundLib();
        std::cerr << "Error LibHandler loadSymbols: " << dlerror() << std::endl;
        throw std::runtime_error("Error LibHandler loadSymbols: couldnt load sound lib functions");
    }
    LOG_DEBUG("Successfully loaded sound symbols");
}

std::unique_ptr<ISoundLib> LibHandler::makeSoundLib()
{
    if (_makerSoundFunc != nullptr && _deleterSoundFunc != nullptr)
        return std::unique_ptr<ISoundLib>(_makerSoundFunc());
    return nullptr;
}

void LibHandler::destroySoundLib(ISoundLib *sLib)
{
    _deleterSoundFunc(sLib);
}

board_size_t LibHandler::getBoardSize() const
{
    return _boardSize;
}