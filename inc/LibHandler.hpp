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
        LIBNCURSES,
        LIBSDL,
        LIBMLX,
    } lib_name_e;

    LibHandler(int, int);
    void openLib(int);
    void closeLib();
    std::unique_ptr<IGraphicLib> switchLib(lib_name_e, std::unique_ptr<IGraphicLib>);

    std::unique_ptr<IGraphicLib> makeGraphicLib(int, int);
    void destroyGraphicLib(std::unique_ptr<IGraphicLib>);

    ~LibHandler();
    LibHandler(const LibHandler &);
    LibHandler &operator=(const LibHandler &);
    // std::unique_ptr<IGraphicLib> makeGraphicHandler(int, int);

  private:
    LibHandler();
    void loadSymbols();

  private:
    int _width;
    int _height;
    void *_lib;
    int _currentLib;

    typedef std::unique_ptr<IGraphicLib> (*makeGraphicLibFunc)(int, int);
    makeGraphicLibFunc _makerFunc;
    typedef void (*destroyGraphicLibFunc)(std::unique_ptr<IGraphicLib>);
    destroyGraphicLibFunc _deleterFunc;

    // typedef void *(*makeGraphicHandlerFunc)(int, int);
    // makeGraphicHandlerFunc _maker;

    static constexpr std::string_view _libPaths[3] = {"./libs/ncurses/libncurses.so", "./libs/sdl/libsdl.so",
                                                      "./libs/mlx/libmlx.so"};
};