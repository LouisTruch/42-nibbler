#include "../inc/Client.hpp"
#include "../inc/LibHandler.hpp"
#include "../inc/Mode/ModeHandler.hpp"
#include "../inc/types.hpp"
#include <iostream>

std::string_view parseIp(const std::string &input);
board_size_t parseInput(const std::string &width, const std::string &height);
void printFormat();

int main(int argc, char **argv)
{
#ifdef DEBUG
    std::cout << "DEBUG MODE" << std::endl;
#endif
    if (argc < 2 || argc > 3)
    {
        printFormat();
        return EXIT_FAILURE;
    }

    // Menu menu;
    // menu.printMenu(boardSize);
    try
    {
        board_size_t boardSize = {10, 10};
        std::unique_ptr<ModeHandler> modeHandler = std::make_unique<ModeHandler>(0);
        std::unique_ptr<LibHandler> libHandler = std::make_unique<LibHandler>(boardSize);
        Client client(std::move(modeHandler), std::move(libHandler));
        client.createGame(boardSize);
        client.startGame();
    }
    catch (const std::exception &e)
    {
        std::cerr << "In main.cpp: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    // Game game(std::move(modeHandler));
    // game.loop();

    // if (argc == 2)
    // {
    //     try
    //     {
    //         Client client(parseIp(argv[1]));
    //         client.readData();
    //         client.instantiatePtrs();
    //     }
    //     catch (const std::exception &e)
    //     {
    //         std::cerr << "In main.cpp: " << e.what() << std::endl;
    //         return EXIT_FAILURE;
    //     }
    // }
    // else if (argc == 3)
    // {
    //     try
    //     {
    //         board_size_t boardSize = parseInput(argv[1], argv[2]);
    //         Menu menu;
    //         menu.printMenu(boardSize);
    //         std::unique_ptr<ModeHandler> modeHandler =
    //             std::make_unique<ModeHandler>(menu.exportGameConfig(), boardSize);
    //         Game game(std::move(modeHandler));
    //         game.loop();
    //     }
    //     catch (const Game::GameOverException &e)
    //     {
    //         std::cerr << "Game: " << e.what() << std::endl;
    //     }
    //     catch (const std::exception &e)
    //     {
    //         std::cerr << "Catch in main.cpp: " << e.what() << std::endl;
    //         return EXIT_FAILURE;
    //     }
    // }
    return EXIT_SUCCESS;
}