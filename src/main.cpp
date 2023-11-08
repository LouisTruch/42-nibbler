#include "../inc/Client.hpp"
#include "../inc/Game.hpp"
#include "../inc/LibHandler.hpp"
#include "../inc/Menu/Menu.hpp"
#include "../inc/ModeHandler.hpp"
#include "../inc/Score.hpp"
#include <iostream>

std::string_view parseIp(const std::string &input);
board_size_t parseInput(const std::string &width, const std::string &height);

#include <stdio.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>

int main(int argc, char **argv)
{
    if (argc < 2 || argc > 3)
    {
        std::cerr << "Correct format is :\n"
                  << "./nibbler [WIDTH] [HEIGHT]\n"
                  << "or\n"
                  << "./nibbler [ipv4 to join]" << std::endl;
        return EXIT_FAILURE;
    }

    if (argc == 2)
    {
        try
        {
            Client client(parseIp(argv[1]));
            client.readData();
            client.instantiatePtrs();
        }
        catch (const std::exception &e)
        {
            std::cerr << "In main.cpp: " << e.what() << std::endl;
            return EXIT_FAILURE;
        }
    }
    else if (argc == 3)
    {
        try
        {
            board_size_t boardSize = parseInput(argv[1], argv[2]);
            Menu menu;
            menu.printMenu(boardSize);
            std::unique_ptr<ModeHandler> modeHandler =
                std::make_unique<ModeHandler>(menu.exportGameConfig(), boardSize);
            Game game(std::move(modeHandler));
            game.loop();
        }
        catch (const Game::GameOverException &e)
        {
            std::cerr << "Game: " << e.what() << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Catch in main.cpp: " << e.what() << std::endl;
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}