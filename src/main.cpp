#include "../inc/Game.hpp"
#include "../inc/Menu/Menu.hpp"
#include "../inc/ModesHandler.hpp"
#include "../inc/types.hpp"
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
    Menu menu;
    int_gameConfig_t config = menu.exportGameConfig();
    ModesHandler modesHandler(config);
    try
    {
        Game game(atoi(argv[1]), atoi(argv[2]), modesHandler);
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