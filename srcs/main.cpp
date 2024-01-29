#include "../inc/Client.hpp"
#include "../inc/LibHandler.hpp"
#include "../inc/Mode/ModeHandler.hpp"
#include "../inc/Network/Server.hpp"
#include "../inc/Network/SocketClient.hpp"
#include "../inc/types.hpp"
#include <iostream> // std::cerr

void printFormat();
int main(int argc, char **argv)
{
#ifdef DEBUG
    std::cerr << "DEBUG MODE" << std::endl;
#endif

    // Use no argument to join a multiplayer game
    if (argc == 1)
    {
        try
        {
            std::unique_ptr<SocketClient> socketClient = std::make_unique<SocketClient>();
            board_size_t boardSize = socketClient->recvBoardData();
            std::unique_ptr<LibHandler> libHandler = std::make_unique<LibHandler>(boardSize);
            Client client(std::move(libHandler), std::move(socketClient));
            client.startGame();
        }
        catch (const std::exception &e)
        {
            std::cerr << "In main.cpp: " << e.what() << std::endl;
            return EXIT_SUCCESS;
        }
        return EXIT_SUCCESS;
    }
    else if (argc == 3)
    {
        try
        {
            board_size_t boardSize = {10, 10};
            std::unique_ptr<ModeHandler> modeHandler = std::make_unique<ModeHandler>(0);
            std::unique_ptr<LibHandler> libHandler = std::make_unique<LibHandler>(boardSize);
            std::unique_ptr<Server> server = std::make_unique<Server>();

            Client client(std::move(libHandler), std::move(server));
            if (server != nullptr)
            {
                client.createGame(boardSize, std::move(modeHandler), true);
            }
            else
            {
                client.createGame(boardSize, std::move(modeHandler), false);
            }
            client.startGame();
        }
        catch (const std::exception &e)
        {
            std::cerr << "In main.cpp: " << e.what() << std::endl;
            return EXIT_SUCCESS;
        }
    }
    else
    {
        printFormat();
        return EXIT_SUCCESS;
    }

    // Menu menu;
    // menu.printMenu(boardSize);

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