#pragma once
#include "../inc/Interface/IGraphicLib.hpp"
#include "../inc/Interface/ISoundLib.hpp"
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
        NO_SOUND = -1,
        SOUNDRAYLIB,
    } lib_sound_e;

  public:
    LibHandler(board_size_t);

  public:
    void openGraphicLib(lib_graphic_e);
    void closeCurrentGraphicLib();
    std::unique_ptr<IGraphicLib> makeGraphicLib();
    void destroyGraphicLib(IGraphicLib *);
    std::unique_ptr<IGraphicLib> switchGraphicLib(lib_graphic_e, std::unique_ptr<IGraphicLib>);

    void openSoundLib(lib_sound_e);
    void closeCurrentSoundLib();
    std::unique_ptr<ISoundLib> makeSoundLib();
    void destroySoundLib(ISoundLib *);
    // TODO :
    // std::unique_ptr<ISoundLib> switchSoundLib(lib_graphic_e, std::unique_ptr<IGraphicLib>);

    ~LibHandler();

  private:
    LibHandler() = delete;
    LibHandler(const LibHandler &) = delete;
    LibHandler &operator=(const LibHandler &) = delete;
    void loadSymbolsGraphicLib();
    void loadSymbolsSoundLib();

  private:
    // TODO: Change those names (Creator,...) watch intra video if no idea
    typedef IGraphicLib *(*makeGraphicLibFunc)(int, int);
    typedef void (*destroyGraphicLibFunc)(IGraphicLib *);
    typedef ISoundLib *(*makeSoundLibFunc)();
    typedef void (*destroySoundLibFunc)(ISoundLib *);

  private:
    int _width;
    int _height;

    lib_graphic_e _currentGraphicLib;
    void *_graphicLibPtr;
    makeGraphicLibFunc _makerGraphicFunc;
    destroyGraphicLibFunc _deleterGraphicFunc;

    lib_sound_e _currentSoundLib;
    void *_soundLibPtr;
    makeSoundLibFunc _makerSoundFunc;
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

    static constexpr int NB_SOUND_LIBS = 1;
    static constexpr std::string_view SOUND_LIB_PATH[1] = {
        "./libs/raylib/sound/libsoundraylib.so",
    };

    // public:
    //   class LibException : public std::exception
    //   {
    //     private:
    //       std::string _msg;

    //     public:
    //       LibException(std::string_view);
    //       virtual const char *what() const throw();
    //       ~LibException() throw() = default;
    //   };
};