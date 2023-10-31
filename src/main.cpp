#include "../inc/Game.hpp"
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char **argv)
{
    try
    {
        checkArgs(argc, argv);
    }
    catch (std::exception const &e)
    {
        std::cerr << "In main.cpp: Parsing error: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    try
    {
        Game game(atoi(argv[1]), atoi(argv[2]));
        game.loop();
    }
    catch (const Game::GameOverException &e)
    {
        std::cerr << "Game: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "In main(): " << e.what() << std::endl;
    }
    return EXIT_SUCCESS;
}