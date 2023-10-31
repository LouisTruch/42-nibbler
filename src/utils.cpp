#include "../inc/Game.hpp"
#include <algorithm>
#include <iostream>

static bool checkSize(const char *str, int min, int max)
{
    errno = 0;
    long int nb = strtol(str, NULL, 10);
    return !(errno == ERANGE || nb < min || nb > max);
}

static bool checkOnlyPosInt(std::string_view sv)
{
    return std::find_if(sv.begin(), sv.end(), [](unsigned char c) { return !std::isdigit(c); }) == sv.end();
}

void checkArgs(int argc, char **argv)
{
    if (argc != 3)
        throw std::logic_error("Correct Format is: [./nibbler] [width] [height]");
    if (!checkOnlyPosInt(argv[1]) || !checkOnlyPosInt(argv[2]))
        throw std::logic_error("Arguments must be positive integers");
    if (!checkSize(argv[1], MIN_WIDTH, MAX_WIDTH) || !checkSize(argv[2], MIN_HEIGHT, MAX_HEIGHT))
        throw std::logic_error("Width [5-47] Height [5-24]");
}