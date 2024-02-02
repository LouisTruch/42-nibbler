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
#ifdef DEBUG
        SOUNDDEBUG,
#endif
    } lib_sound_e;

  public:
    LibHandler(board_size_t);
    ~LibHandler();
    LibHandler() = delete;
    LibHandler(const LibHandler &) = delete;
    LibHandler &operator=(const LibHandler &) = delete;

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
    std::unique_ptr<ISoundLib> switchSoundLib(lib_sound_e, std::unique_ptr<ISoundLib>);
    board_size_t getBoardSize() const;

  private:
    void loadSymbolsGraphicLib();
    void loadSymbolsSoundLib();

  private:
    // TODO: Change those names (Creator,...)
    typedef IGraphicLib *(*makeGraphicLibFunc)(int, int);
    typedef void (*destroyGraphicLibFunc)(IGraphicLib *);
    typedef ISoundLib *(*makeSoundLibFunc)();
    typedef void (*destroySoundLibFunc)(ISoundLib *);

  private:
    board_size_t _boardSize;

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
    static constexpr int NB_SOUND_LIBS = 1;
    static constexpr std::string_view SOUND_LIB_PATH[1] = {
        "./libs/raylib/sound/libsoundraylib.so",
    };
#else
    static constexpr int NB_GRAPHIC_LIBS = 4;
    static constexpr std::string_view GRAPHIC_LIB_PATH[4] = {
        "./libs/ncurses/libncurses.so",
        "./libs/sdl/libsdl.so",
        "./libs/raylib/libraylib.so",
        "./libs/debug/libdebug.so",
    };
    static constexpr int NB_SOUND_LIBS = 2;
    static constexpr std::string_view SOUND_LIB_PATH[2] = {
        "./libs/raylib/sound/libsoundraylib.so",
        "./libs/debug/sound/libsounddebug.so",
    };
#endif
};