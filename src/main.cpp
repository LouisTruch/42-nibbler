#include "../inc/Game.hpp"
#include "../inc/LibHandler.hpp"
#include "../inc/Menu/Menu.hpp"
#include "../inc/ModeHandler.hpp"
#include "../inc/Score.hpp"
#include <iostream>

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
    menu.printMenu();
    std::unique_ptr<ModeHandler> modeHandler =
        std::make_unique<ModeHandler>(menu.exportGameConfig(), atoi(argv[1]), atoi(argv[2]));
    try
    {
        // std::unique_ptr<LibHandler> libHandler = std::make_unique<LibHandler>(atoi(argv[1]), atoi(argv[2]));
        Game game(atoi(argv[1]), atoi(argv[2]), std::move(modeHandler));
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