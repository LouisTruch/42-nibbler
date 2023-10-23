#pragma once
#include "../inc/IGraphicLib.hpp"
#include <memory>
#include <string> // std::string_view

#define NB_LIBS 3

class LibHandler
{
public:
    typedef enum
    {
        LIBSDL,
        LIB1,
        LIB2,
    } lib_name_e;

    LibHandler();
    void switchLib(lib_name_e);

    std::unique_ptr<IGraphicLib> makeGraphicLib(int, int);
    void destroyGraphicLib(std::unique_ptr<IGraphicLib>);

    ~LibHandler();
    LibHandler(const LibHandler &);
    LibHandler &operator=(const LibHandler &);
    // std::unique_ptr<IGraphicLib> makeGraphicHandler(int, int);

private:
    void loadSymbols();

private:
    void *_lib;

    typedef u_int8_t current_lib_t;
    current_lib_t _numCurrentLib;
    typedef std::unique_ptr<IGraphicLib> (*makeGraphicLibFunc)(int, int);
    makeGraphicLibFunc _makerFunc;
    typedef void (*destroyGraphicLibFunc)(std::unique_ptr<IGraphicLib>);
    destroyGraphicLibFunc _deleterFunc;

    // typedef void *(*makeGraphicHandlerFunc)(int, int);
    // makeGraphicHandlerFunc _maker;

    static constexpr std::string_view _libPaths[3] = {"libs/sdl/libsdl.so", "a", "b"};
};