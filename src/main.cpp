#include "../inc/Client.hpp"
#include "../inc/Game.hpp"
#include "../inc/LibHandler.hpp"
#include "../inc/Menu/Menu.hpp"
#include "../inc/ModeHandler.hpp"
#include "../inc/Score.hpp"
#include <iostream>

// Port 7777 for multiplayer
#include <cstring>
#include <netinet/in.h> // struct sockaddr_in
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h> // close()

int main(int argc, char **argv)
{
    if (!argv[2])
    {
        try
        {
            // Need to add parsing when only joining a game
            Client client(argv[1]);
            client.readData();
            // client.readInitData();
            client.instantiatePtrs();
        }
        catch (const std::exception &e)
        {
            std::cerr << "In main.cpp: " << e.what() << std::endl;
        }
        return 0;
    }
    // else
    // {
    //     server();
    //     return 0;
    // }
    // try
    // {
    //     checkArgs(argc, argv);
    // }
    // catch (std::exception const &e)
    // {
    //     std::cerr << "In main.cpp: Parsing error: " << e.what() << std::endl;
    //     exit(EXIT_FAILURE);
    // }

    // Menu menu;
    // menu.printMenu();
    // std::unique_ptr<ModeHandler> modeHandler =
    //     std::make_unique<ModeHandler>(menu.exportGameConfig(), atoi(argv[1]), atoi(argv[2]));
    std::unique_ptr<ModeHandler> modeHandler = std::make_unique<ModeHandler>();
    try
    {
        // std::unique_ptr<LibHandler> libHandler = std::make_unique<LibHandler>(atoi(argv[1]), atoi(argv[2]));
        Game game(std::move(modeHandler));
        game.loop();
    }
    catch (const Game::GameOverException &e)
    {
        std::cerr << "Game: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "In main.cpp: " << e.what() << std::endl;
    }
    return EXIT_SUCCESS;
}