#pragma once
#include "../inc/IGraphicLib.hpp"
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
        LIBNCURSES,
        LIBSDL,
        LIBRAYLIB,
        LIBSOUND,
    } lib_name_e;

    typedef enum
    {
        GRAPHIC,
        SOUND,
    } lib_type_e;

    // For loading graphic lib
    LibHandler(int, int);
    void openLib(lib_type_e, int);
    void openSoundLib(int);

    void closeLib(lib_type_e);
    std::unique_ptr<IGraphicLib> switchGraphicLib(lib_name_e, std::unique_ptr<IGraphicLib>);

    std::unique_ptr<IGraphicLib> makeGraphicLib(int, int);
    void destroyGraphicLib(std::unique_ptr<IGraphicLib>);

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
    int _currentGraphicLib;
    int _currentSoundLib;
    void *_graphicLibPtr;
    void *_soundLibPtr;

    typedef std::unique_ptr<IGraphicLib> (*makeGraphicLibFunc)(int, int);
    makeGraphicLibFunc _makerGraphicFunc;
    typedef void (*destroyGraphicLibFunc)(std::unique_ptr<IGraphicLib>);
    destroyGraphicLibFunc _deleterGraphicFunc;

    typedef std::unique_ptr<ISoundLib> (*makeSoundLibFunc)();
    makeSoundLibFunc _makerSoundFunc;
    typedef void (*destroySoundLibFunc)(std::unique_ptr<ISoundLib>);
    destroySoundLibFunc _deleterSoundFunc;

    static constexpr int NB_GRAPHIC_LIBS = 3;
    static constexpr std::string_view LIB_PATH[4] = {"./libs/ncurses/libncurses.so", "./libs/sdl/libsdl.so",
                                                     "./libs/raylib/libraylib.so",
                                                     "./libs/raylib/sound/libsoundraylib.so"};

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