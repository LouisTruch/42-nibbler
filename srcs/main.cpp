#include "../inc/Client.hpp"
#include "../inc/LibHandler.hpp"
#include "../inc/Menu/Menu.hpp"
#include "../inc/Mode/ModeHandler.hpp"
#include "../inc/Network/Server.hpp"
#include "../inc/Network/SocketClient.hpp"
#include "../inc/types.hpp"
#include <iostream> // std::cerr

#ifdef DEBUG
void launchDebug()
{
    try
    {
        int_GameConfig_t config = 0;
        board_size_t boardSize = {10, 10};
        bool multiplayer = false;

        std::unique_ptr<ModeHandler> modeHandler = std::make_unique<ModeHandler>(config);
        std::unique_ptr<LibHandler> libHandler = std::make_unique<LibHandler>(boardSize);
        std::unique_ptr<Server> server = nullptr;
        if (config & 0x20)
            server = std::make_unique<Server>();

        Client client(std::move(libHandler), std::move(server));
        client.createGame(boardSize, std::move(modeHandler), multiplayer);
        client.startGameLoop();
    }
    catch (const std::exception &e)
    {
        std::cerr << "In launchDebug(): " << e.what() << std::endl;
    }
}
#endif // DEBUG

int joinServer() noexcept
{
    try
    {
        std::unique_ptr<SocketClient> socketClient = std::make_unique<SocketClient>();
        board_size_t boardSize = socketClient->recvBoardData();
        std::unique_ptr<LibHandler> libHandler = std::make_unique<LibHandler>(boardSize);
        Client client(std::move(libHandler), std::move(socketClient));
        client.joinGame();
    }
    catch (const std::exception &e)
    {
        std::cerr << "In main.cpp: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void printFormat();
board_size_t parseInput(const std::string &width, const std::string &height);
int main(int argc, char **argv)
{
#ifdef DEBUG
    std::cerr << "DEBUG MODE" << std::endl;
    launchDebug();
    return 0;
#endif

    if (argc == 1)
        return joinServer();
    else if (argc == 3)
    {
        try
        {
            board_size_t boardSize = parseInput(argv[1], argv[2]);
            int_GameConfig_t config;
            {
                Menu menu;
                menu.printMenu(boardSize);
                config = menu.exportGameConfig();
            }
            bool multiplayer = false;
            if (!(config & 0x8))
                multiplayer = true;

            std::unique_ptr<ModeHandler> modeHandler = std::make_unique<ModeHandler>(config);
            std::unique_ptr<LibHandler> libHandler = std::make_unique<LibHandler>(boardSize);
            std::unique_ptr<Server> server = nullptr;
            if (config & 0x20)
                server = std::make_unique<Server>();

            Client client(std::move(libHandler), std::move(server));
            client.createGame(boardSize, std::move(modeHandler), multiplayer);
            client.startGameLoop();
        }
        catch (const std::exception &e)
        {
            std::cerr << "In main.cpp: " << e.what() << std::endl;
            return EXIT_FAILURE;
        }
    }
    else
    {
        printFormat();
        return EXIT_SUCCESS;
    }
    return EXIT_SUCCESS;
}