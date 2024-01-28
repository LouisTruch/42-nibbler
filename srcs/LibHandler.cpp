#include "../inc/LibHandler.hpp"

#include "../inc/Log/Logger.hpp"

#include <dlfcn.h>
#include <stdexcept>

LibHandler::LibHandler(board_size_t boardSize)
    : _width(boardSize.x), _height(boardSize.y), _currentGraphicLib(NO_LIB), _currentSoundLib(-1),
      _graphicLibPtr(nullptr), _soundLibPtr(nullptr)
{
#ifndef DEBUG
    openGraphicLib(LIBSDL);
#else
    openGraphicLib(LIBRAYLIB);
#endif
    loadSymbolsGraphicLib();
}

LibHandler::~LibHandler()
{
    closeCurrentGraphicLib();
    if (_graphicLibPtr)
    {
        dlclose(_graphicLibPtr);
    }
    if (_soundLibPtr)
    {
        dlclose(_soundLibPtr);
    }
    LOG_DEBUG("Destructing LibHandler");
}

void LibHandler::openGraphicLib(lib_graphic_e libChoice)
{
    _graphicLibPtr = dlopen(GRAPHIC_LIB_PATH[libChoice].data(), RTLD_LAZY);
    if (!_graphicLibPtr)
        throw std::runtime_error("Error LibHandler(): could not load .so object");

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

void LibHandler::openSoundLib(int libChoice)
{
    _soundLibPtr = dlopen(GRAPHIC_LIB_PATH[libChoice].data(), RTLD_LAZY);
    if (!_soundLibPtr)
        throw std::runtime_error("Error LibHandler(): couldnt load .so object");
    _currentSoundLib = libChoice;
    loadSymbolsSoundLib();
    LOG_DEBUG("Successfully loaded SOUND .so object");
    // if (libChoice == _currentSoundLib)
    //     return;

    // if (0 < libChoice && libChoice >= NB_SOUND_LIBS)
    //     return;

    // if (_soundLibPtr)
    //     closeLib(SOUND);
    // openLib(SOUND, libChoice);
    // loadSymbolsSoundLib();
}

void LibHandler::closeCurrentGraphicLib()
{
    if (dlclose(_graphicLibPtr))
        throw std::runtime_error("Error LibHandler->closeLib(): Couldnt close lib");
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
    // _deleterGraphicFunc(gLib);
    // closeLib(GRAPHIC);
    // openGraphicLib(libChoice);
    // loadSymbolsGraphicLib();
    // return _makerGraphicFunc(_width + 2, _height + 2);
}

void LibHandler::loadSymbolsGraphicLib(void)
{
    _makerGraphicFunc = (makeGraphicLibFunc)dlsym(_graphicLibPtr, "makeGraphicLib");
    _deleterGraphicFunc = (destroyGraphicLibFunc)dlsym(_graphicLibPtr, "destroyGraphicLib");
    if (!_makerGraphicFunc || !_deleterGraphicFunc)
        throw std::runtime_error("Error LibHandler loadSymbols: couldnt load lib functions");
    LOG_DEBUG("Successfully loaded graphic symbols");
}

void LibHandler::loadSymbolsSoundLib(void)
{
    _makerSoundFunc = (makeSoundLibFunc)dlsym(_soundLibPtr, "makeSoundLib");
    _deleterSoundFunc = (destroySoundLibFunc)dlsym(_soundLibPtr, "destroySoundLib");
    if (!_makerSoundFunc || !_deleterSoundFunc)
        throw std::runtime_error("Error LibHandler loadSymbols: couldnt load lib functions");
    LOG_DEBUG("Successfully loaded sound symbols");
}

std::unique_ptr<IGraphicLib> LibHandler::makeGraphicLib(void)
{
    return std::unique_ptr<IGraphicLib>(_makerGraphicFunc(_width, _height));
    // return _makerGraphicFunc(_width, _height);
}

void LibHandler::destroyGraphicLib(IGraphicLib *gLib)
{
    _deleterGraphicFunc(gLib);
}

std::unique_ptr<ISoundLib> LibHandler::makeSoundLib()
{
    return _makerSoundFunc();
}

void LibHandler::destroySoundLib(std::unique_ptr<ISoundLib> sLib)
{
    _deleterSoundFunc(std::move(sLib));
}