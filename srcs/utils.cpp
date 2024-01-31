#include "../inc/Game/Game.hpp"
#include "../inc/types.hpp"
#include <algorithm> // std::all_of
#include <iostream>  // std::cerr
#include <stdexcept> // std::logic_error

bool isStrOnlyDigits(const std::string &str)
{
    return std::all_of(str.begin(), str.end(), ::isdigit);
}

bool isIntInRange(const std::string &str, int min, int max)
{
    return !(min > std::stoi(str) || std::stoi(str) > max);
}

#define MAX_WIDTH 47
#define MIN_WIDTH 5
#define MAX_HEIGHT 26
#define MIN_HEIGHT 5
board_size_t parseInput(const std::string &width, const std::string &height)
{
    if (!isStrOnlyDigits(width) || !isStrOnlyDigits(height))
        throw std::logic_error("In utils.cpp: parseInput(): WIDTH and HEIGHT can only contain digits");
    if (!isIntInRange(width, MIN_WIDTH, MAX_WIDTH))
        throw std::logic_error("In utils.cpp: parseInput(): WIDTH must be between " + std::to_string(MIN_WIDTH) +
                               " and " + std::to_string(MAX_WIDTH));
    if (!isIntInRange(height, MIN_HEIGHT, MAX_HEIGHT))
        throw std::logic_error("In utils.cpp: parseInput(): HEIGHT must be between " + std::to_string(MIN_HEIGHT) +
                               " and " + std::to_string(MAX_HEIGHT));
    return {std::stoi(width), std::stoi(height)};
}

void printFormat()
{
    std::cerr << "Correct format is :\n"
              << "./nibbler [WIDTH] [HEIGHT]\n"
              << "or\n"
              << "./nibbler [ipv4 to join]" << std::endl;
}