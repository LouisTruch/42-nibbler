#pragma once
#include "../inc/Interface/IGraphicLib.hpp"
#include "../inc/ISoundLib.hpp"
#include <memory>
#include <string>
#include <string_view>

class Game;

class LibHandler
{
  public:
    typedef enum
    {
        NO_LIB = -1,
        LIBNCURSES,
        LIBSDL,
        LIBRAYLIB,
#ifdef DEBUG
        LIBDEBUG,
#endif
    } lib_graphic_e;

    typedef enum
    {
        GRAPHIC,
        SOUND,
    } lib_type_e;

    // For loading graphic lib
    LibHandler(board_size_t);
    void openGraphicLib(lib_graphic_e);
    void openSoundLib(int);
    void closeCurrentGraphicLib();

    void closeLib(lib_type_e);
    std::unique_ptr<IGraphicLib> makeGraphicLib();
    void destroyGraphicLib(IGraphicLib *);
    std::unique_ptr<IGraphicLib> switchGraphicLib(lib_graphic_e, std::unique_ptr<IGraphicLib>);

    std::unique_ptr<ISoundLib> makeSoundLib();
    void destroySoundLib(std::unique_ptr<ISoundLib>);

    ~LibHandler();
    LibHandler(const LibHandler &) = delete;
    LibHandler &operator=(const LibHandler &) = delete;

  private:
    LibHandler() = delete;
    void loadSymbolsGraphicLib();
    void loadSymbolsSoundLib();

  private:
    int _width;
    int _height;
    lib_graphic_e _currentGraphicLib;
    int _currentSoundLib;
    void *_graphicLibPtr;
    void *_soundLibPtr;

    typedef IGraphicLib *(*makeGraphicLibFunc)(int, int);
    makeGraphicLibFunc _makerGraphicFunc;

    typedef void (*destroyGraphicLibFunc)(IGraphicLib *);
    destroyGraphicLibFunc _deleterGraphicFunc;

    typedef std::unique_ptr<ISoundLib> (*makeSoundLibFunc)();
    makeSoundLibFunc _makerSoundFunc;

    typedef void (*destroySoundLibFunc)(std::unique_ptr<ISoundLib>);
    destroySoundLibFunc _deleterSoundFunc;

#ifndef DEBUG
    static constexpr int NB_GRAPHIC_LIBS = 3;
    static constexpr std::string_view GRAPHIC_LIB_PATH[3] = {
        "./libs/ncurses/libncurses.so",
        "./libs/sdl/libsdl.so",
        "./libs/raylib/libraylib.so",
    };
#else
    static constexpr int NB_GRAPHIC_LIBS = 4;
    static constexpr std::string_view GRAPHIC_LIB_PATH[4] = {
        "./libs/ncurses/libncurses.so",
        "./libs/sdl/libsdl.so",
        "./libs/raylib/libraylib.so",
        "./libs/debug/libdebug.so",
    };
#endif

  public:
    class LibException : public std::exception
    {
      private:
        std::string _msg;

      public:
        LibException(std::string_view);
        virtual const char *what() const throw();
        ~LibException() throw() = default;
    };
};