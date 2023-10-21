#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include "../inc/Game.hpp"
#include "../inc/IGraphicHandler.hpp"

#define MIN_WIDTH 5
#define MAX_WIDTH 50
#define MIN_HEIGHT 5
#define MAX_HEIGHT 50

static bool checkSize(const char *str)
{
    errno = 0;
    long int nb = strtol(str, NULL, 10);
    return !(errno == ERANGE || nb < MIN_WIDTH || nb > MAX_WIDTH);
}

static bool checkOnlyPosInt(std::string_view sv)
{
    return std::find_if(sv.begin(),
                        sv.end(), [](unsigned char c)
                        { return !std::isdigit(c); }) == sv.end();
}

static void checkArgs(int argc, char **argv)
{
    if (argc != 3)
        throw std::logic_error("Correct Format is: [./nibbler] [width] [height]");
    for (auto &&str : std::vector<std::string_view>{argv + 1, argv + argc})
    {
        if (!checkOnlyPosInt(str))
            throw std::logic_error("Arguments must be positive integers");
        if (!checkSize(str.data()))
            throw std::logic_error("Width [5-50] Height [5-50]");
    }
}

#include <dlfcn.h>
#include <memory>

int main(int argc, char **argv)
{
    try
    {
        checkArgs(argc, argv);
    }
    catch (std::exception const &e)
    {
        std::cerr << "In main.cpp: Parsing error: " << e.what() << std::endl;
        exit(1);
    }
    // Game game(atoi(argv[1]), atoi(argv[2]));
    auto closeLib = [](void *lib)
    {
        dlclose(lib);
    };
    typedef void *(*FuncPtr)(int, int);
    using handlePtr = std::unique_ptr<void, decltype(closeLib)>;

    handlePtr lib(dlopen("libs/sdl/libsdl.so", RTLD_LAZY), closeLib);
    // void *lib = dlopen("libs/sdl/libsdl.so", RTLD_NOW);
    // handlePtr(lib, closeLib);
    if (!lib)
    {
        std::cout << "ff\n"
                  << dlerror();
    }
    else
    {
        std::cout << "no ff\n";
        FuncPtr func = (FuncPtr)dlsym(lib.get(), "makeGraphicHandler");
        if (func)
        {
            std::cout << "oui\n";
            void *ptr = func(100, 100);
        }
    }
    return 0;
}