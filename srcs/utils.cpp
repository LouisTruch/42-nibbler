#include "../inc/types.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <regex>
#include <stdexcept>

std::string_view parseIp(const std::string &input)
{
    std::regex ipv4Pattern("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
    if (std::regex_match(input, ipv4Pattern))
        return input;
    throw std::runtime_error("In utils.cpp: parseIp(): invalid IPv4");
}

bool isStrOnlyDigits(const std::string &str)
{
    return std::all_of(str.begin(), str.end(), ::isdigit);
}

bool isIntInRange(const std::string &str, int min, int max)
{
    return !(min > std::stoi(str) || std::stoi(str) > max);
}

// TODO : Fix this function
board_size_t parseInput(const std::string &width, const std::string &height)
{
    // if (!isStrOnlyDigits(width) || !isStrOnlyDigits(height))
    //     throw std::logic_error("In utils.cpp: parseInput(): WIDTH and HEIGHT can only contain digits");
    // if (!isIntInRange(width, MIN_WIDTH, MAX_WIDTH))
    //     throw std::logic_error("In utils.cpp: parseInput(): HEIGHT must be between " + std::to_string(MIN_WIDTH) +
    //                            " and " + std::to_string(MAX_WIDTH));
    // if (!isIntInRange(height, MIN_HEIGHT, MAX_HEIGHT))
    //     throw std::logic_error("In utils.cpp: parseInput(): HEIGHT must be between " + std::to_string(MIN_HEIGHT) +
    //                            " and " + std::to_string(MAX_HEIGHT));
    return {std::stoi(width), std::stoi(height)};
}

void printFormat()
{
    std::cerr << "Correct format is :\n"
              << "./nibbler [WIDTH] [HEIGHT]\n"
              << "or\n"
              << "./nibbler [ipv4 to join]" << std::endl;
}