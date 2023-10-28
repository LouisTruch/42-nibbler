#include "../inc/LibHandler.hpp"
#include <dlfcn.h>
#include <iostream>
#include <stdexcept>

LibHandler::LibHandler(int width, int height) : _width(width), _height(height), _currentLib(-1)
{
    openLib(LIBRAYLIB);
    loadSymbols();
}

void LibHandler::openLib(int libChoice)
{
    _currentLib = libChoice;
    _lib = dlopen(_libPaths[_currentLib].data(), RTLD_LAZY);
    if (!_lib)
        throw std::runtime_error("Error LibHandler(): couldnt load .so object");
}

void LibHandler::closeLib()
{
    if (dlclose(_lib))
        throw std::runtime_error("Error LibHandler->closeLib(): Couldnt close lib");
}

LibHandler::~LibHandler()
{
    dlclose(_lib);
}

std::unique_ptr<IGraphicLib> LibHandler::switchLib(lib_name_e libChoice, std::unique_ptr<IGraphicLib> gLib)
{
    if (libChoice == _currentLib)
        return gLib;

    if (0 < libChoice && libChoice >= NB_LIBS)
        return gLib;

    _deleterFunc(std::move(gLib));
    closeLib();
    openLib(libChoice);
    loadSymbols();
    return _makerFunc(_width, _height);
}

void LibHandler::loadSymbols(void)
{
    _makerFunc = (makeGraphicLibFunc)dlsym(_lib, "makeGraphicLib");
    _deleterFunc = (destroyGraphicLibFunc)dlsym(_lib, "destroyGraphicLib");
    if (!_makerFunc || !_deleterFunc)
        throw std::runtime_error("Error LibHandler loadSymbols: couldnt load lib functions");
}

std::unique_ptr<IGraphicLib> LibHandler::makeGraphicLib(int width, int height)
{
    return _makerFunc(width, height);
}

void LibHandler::destroyGraphicLib(std::unique_ptr<IGraphicLib> gLib)
{
    _deleterFunc(std::move(gLib));
}

LibHandler::LibHandler(const LibHandler &other)
{
    *this = other;
}

LibHandler &LibHandler::operator=(const LibHandler &other)
{
    if (&other == this)
        return *this;
    _lib = other._lib;
    _currentLib = other._currentLib;
    _makerFunc = other._makerFunc;
    _deleterFunc = other._deleterFunc;
    return *this;
}

LibHandler::LibHandler()
{
}