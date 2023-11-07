#include "../inc/LibHandler.hpp"
#include <dlfcn.h>
#include <iostream>
#include <stdexcept>

LibHandler::LibHandler(int width, int height)
    : _width(width), _height(height), _currentGraphicLib(-1), _currentSoundLib(-1), _graphicLibPtr(NULL),
      _soundLibPtr(NULL)
{
    openLib(GRAPHIC, LIBSDL);
    loadSymbolsGraphicLib();
}

void LibHandler::openLib(lib_type_e type, int libChoice)
{
    if (type == GRAPHIC)
    {
        _currentGraphicLib = libChoice;
        _graphicLibPtr = dlopen(LIB_PATH[_currentGraphicLib].data(), RTLD_LAZY);
        if (!_graphicLibPtr)
            throw std::runtime_error("Error LibHandler(): couldnt load .so object");
    }
    else if (type == SOUND)
    {
        _currentSoundLib = libChoice;
        _soundLibPtr = dlopen(LIB_PATH[_currentSoundLib].data(), RTLD_LAZY);
        if (!_soundLibPtr)
            throw std::runtime_error("Error LibHandler(): couldnt load .so object");
        loadSymbolsSoundLib();
    }
}

void LibHandler::closeLib(lib_type_e type)
{
    if (type == GRAPHIC)
    {
        if (dlclose(_graphicLibPtr))
            throw std::runtime_error("Error LibHandler->closeLib(): Couldnt close lib");
    }
    else if (type == SOUND)
    {
        if (dlclose(_soundLibPtr))
            throw std::runtime_error("Error LibHandler->closeLib(): Couldnt close lib");
    }
}

LibHandler::~LibHandler()
{
    if (_graphicLibPtr)
        dlclose(_graphicLibPtr);
    if (_soundLibPtr)
        dlclose(_soundLibPtr);
}

std::unique_ptr<IGraphicLib> LibHandler::switchGraphicLib(lib_name_e libChoice, std::unique_ptr<IGraphicLib> gLib)
{
    if (libChoice == _currentGraphicLib)
        return gLib;

    if (0 < libChoice && libChoice >= NB_GRAPHIC_LIBS)
        return gLib;

    _deleterGraphicFunc(std::move(gLib));
    closeLib(GRAPHIC);
    openLib(GRAPHIC, libChoice);
    loadSymbolsGraphicLib();
    return _makerGraphicFunc(_width + 2, _height + 2);
}

void LibHandler::loadSymbolsGraphicLib(void)
{
    _makerGraphicFunc = (makeGraphicLibFunc)dlsym(_graphicLibPtr, "makeGraphicLib");
    _deleterGraphicFunc = (destroyGraphicLibFunc)dlsym(_graphicLibPtr, "destroyGraphicLib");
    if (!_makerGraphicFunc || !_deleterGraphicFunc)
        throw std::runtime_error("Error LibHandler loadSymbols: couldnt load lib functions");
}

void LibHandler::loadSymbolsSoundLib(void)
{
    _makerSoundFunc = (makeSoundLibFunc)dlsym(_soundLibPtr, "makeSoundLib");
    _deleterSoundFunc = (destroySoundLibFunc)dlsym(_soundLibPtr, "destroySoundLib");
    if (!_makerSoundFunc || !_deleterSoundFunc)
        throw std::runtime_error("Error LibHandler loadSymbols: couldnt load lib functions");
}

std::unique_ptr<IGraphicLib> LibHandler::makeGraphicLib(int width, int height)
{
    return _makerGraphicFunc(width, height);
}

void LibHandler::destroyGraphicLib(std::unique_ptr<IGraphicLib> gLib)
{
    _deleterGraphicFunc(std::move(gLib));
}

std::unique_ptr<ISoundLib> LibHandler::makeSoundLib()
{
    return _makerSoundFunc();
}

void LibHandler::destroySoundLib(std::unique_ptr<ISoundLib> sLib)
{
    _deleterSoundFunc(std::move(sLib));
}