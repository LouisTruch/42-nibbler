#include "../inc/LibHandler.hpp"
#include <dlfcn.h>
#include <stdexcept>

LibHandler::LibHandler()
{
    _lib = dlopen(_libPaths[LIBSDL].data(), RTLD_LAZY);
    if (!_lib)
        throw std::runtime_error("Error LibHandler(): couldnt load .so object");
    _numCurrentLib = LIBSDL;
    loadSymbols();
}

LibHandler::~LibHandler()
{
    dlclose(_lib);
}

void LibHandler::switchLib(lib_name_e libChoice)
{
    if (libChoice == _numCurrentLib)
        throw std::runtime_error("Error LibHandler->SwitchLib(): This lib is already loaded");

    if (0 < libChoice && libChoice >= NB_LIBS)
        throw std::runtime_error("Error LibHandler->SwitchLib(): Library does not exist");

    if (dlclose(_lib))
        throw std::runtime_error("Error LibHandler->SwitchLib(): Error closing library");

    _lib = dlopen(_libPaths[libChoice].data(), RTLD_LAZY);
    if (!_lib)
        throw std::runtime_error("Error LibHandler->SwitchLib(): couldnt load .so object");

    _numCurrentLib = libChoice;
    loadSymbols();
}

void LibHandler::loadSymbols(void)
{
    _makerFunc = (makeGraphicLibFunc)dlsym(_lib, "makeGraphicLib");
    _deleterFunc = (destroyGraphicLibFunc)dlsym(_lib, "destroyGraphicLib");
    if (!_makerFunc || !_deleterFunc)
        throw std::runtime_error("Error LibHandler loadSymbols: couldnt load lib functions");
    // _maker = (makeGraphicHandlerFunc)dlsym(_lib, "makeGraphicHandler");
    // if (!_maker)
    // throw std::runtime_error("Error LibHandler loadSymbols: couldnt load makeGraphicHandler");
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
    _numCurrentLib = other._numCurrentLib;
    _makerFunc = other._makerFunc;
    _deleterFunc = other._deleterFunc;
    return *this;
}

// std::unique_ptr<IGraphicLib> LibHandler::makeGraphicHandler(int width, int height)
// {
//     return std::unique_ptr<IGraphicLib>(reinterpret_cast<IGraphicLib *>(_maker(width, height)));
// }
