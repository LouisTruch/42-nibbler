#include "../inc/Client.hpp"
#include "../inc/Log/Logger.hpp"

Client::Client(std::unique_ptr<ModeHandler> modeHandler, std::unique_ptr<LibHandler> libHandler)
    : _modeHandler(std::move(modeHandler)), _libHandler(std::move(libHandler)),
      _graphicLib(_libHandler->makeGraphicLib()), _game(nullptr)
{
    LOG_DEBUG("Constructing Client");
}

Client::~Client()
{
    LOG_DEBUG("Destructing Client");
}

void Client::createGame(board_size_t boardSize)
{
    _game = std::make_unique<Game>(boardSize, std::move(_modeHandler));
}

void Client::startGame()
{
    while (1)
    {
        _graphicLib->registerPlayerInput();
        player_input_t playerInput = _graphicLib->getPlayerInput(0);
        input_type_e inputType = checkPlayerInput(playerInput);
        if (inputType == QUIT)
            break;
        else if (inputType == SWAP_LIB)
        {
            handleLibSwitch(playerInput);
        }
        else if (inputType == DIRECTION)
        {
            if (!checkIfOppositeDirection(playerInput))
                _game->getP0()->setNextDirection(playerInput);
        }

        _game->playTurn();
        render();
    }
    LOG_DEBUG("Exiting Client::startGame()");
}

Client::input_type_e Client::checkPlayerInput(player_input_t playerInput) noexcept
{
    switch (playerInput)
    {
    case INPUT_QUIT:
        return QUIT;

#ifdef DEBUG
    case INPUT_SWAP_LIBDEBUG:
        [[fallthrough]];
#endif
    case INPUT_SWAP_LIBNCURSES:
        [[fallthrough]];
    case INPUT_SWAP_LIBSDL:
        [[fallthrough]];
    case INPUT_SWAP_LIBRAYLIB:
        return SWAP_LIB;

    case INPUT_LEFT:
        [[fallthrough]];
    case INPUT_RIGHT:
        [[fallthrough]];
    case INPUT_UP:
        [[fallthrough]];
    case INPUT_DOWN:
        return DIRECTION;

    case INPUT_DEFAULT:
        [[fallthrough]];
    default:
        break;
    }
    return UNHANDLED;
}

LibHandler::lib_graphic_e Client::inputToLibNum(player_input_t input) noexcept
{
    switch (input)
    {
    case INPUT_SWAP_LIBNCURSES:
        return LibHandler::LIBNCURSES;
    case INPUT_SWAP_LIBSDL:
        return LibHandler::LIBSDL;
    case INPUT_SWAP_LIBRAYLIB:
        return LibHandler::LIBRAYLIB;
#ifdef DEBUG
    case INPUT_SWAP_LIBDEBUG:
        return LibHandler::LIBDEBUG;
#endif
    default:
        return LibHandler::NO_LIB;
    }
}

void Client::handleLibSwitch(player_input_t playerInput)
{
    LibHandler::lib_graphic_e libNum = inputToLibNum(playerInput);
    _graphicLib = _libHandler->switchGraphicLib(libNum, std::move(_graphicLib));
}

bool Client::checkIfOppositeDirection(player_input_t playerInput)
{
    if (playerInput + (player_input_t)_game->getP0()->getPrevDirection() == 0)
        return true;
    return false;
}

void Client::render()
{
    _graphicLib->clearBoard();
    _graphicLib->drawPlayer(*_game->getP0());
    // _graphicLib->drawPlayer(*_game->getP1());
    _graphicLib->drawFood(*_game->getFood());
}

// Old Client class
// Client::Client(std::string_view ip)
//     : _isSound(false), _libHandler(nullptr), _graphicHandler(nullptr), _soundHandler(nullptr)
// {
//     _fd = socket(AF_INET, SOCK_STREAM, 0);
//     if ((_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
//         throw std::runtime_error("In Client(): socket() error");

//     struct sockaddr_in servAddr;
//     memset(&servAddr, 0, sizeof(servAddr));
//     servAddr.sin_family = AF_INET;
//     servAddr.sin_port = htons(PORT);

//     // Change address below by argv1
//     if (inet_pton(AF_INET, ip.data(), &servAddr.sin_addr) <= 0)
//         throw std::runtime_error("In Client(): inet_pton() error");
//     if (connect(_fd, (struct sockaddr *)&servAddr, sizeof(servAddr)))
//         throw std::runtime_error("In Client(): connect() error");

//     // Ignore SIGPIPE to end properly execution when server closed and recv() sends a SIGPIPE
//     signal(SIGPIPE, SIG_IGN);
// }
// void Client::instantiatePtrs()
// {
//     // _libHandler = std::make_unique<LibHandler>(_width, _height);
//     // _graphicHandler = _libHandler->makeGraphicLib(_width + 2, _height + 2);
//     if (_isSound)
//     {
//         // _libHandler->openLib(LibHandler::SOUND, LibHandler::LIBSOUND);
//         _soundHandler = _libHandler->makeSoundLib();
//     }
// }

// void Client::readData()
// {
//     std::string bufferRecv, bufferSend;
//     int byteRead = 1;
//     int byteSent;
//     bufferRecv.resize(5000);
//     while (byteRead > 0)
//     {
//         byteRead = recv(_fd, &bufferRecv[0], bufferRecv.size(), 0);
//         if (byteRead < 0)
//             throw std::runtime_error("In Client::readData(): recv() host is probably offline");

//         if (bufferRecv[0] == 'i')
//         {
//             readInitData(bufferRecv);
//             instantiatePtrs();
//             continue;
//         }
//         else if (bufferRecv[0] == 'm')
//         {
//             readMessage(bufferRecv);
//             break;
//         }
//         try
//         {
//             if (bufferRecv[0] == 'p' && _graphicHandler != nullptr)
//                 readGameData(bufferRecv);
//         }
//         catch (const std::exception &e)
//         {
//             std::cerr << "In Client::readData(): " << e.what() << std::endl;
//         }

//         _graphicHandler->registerPlayerInput();
//         if (_graphicHandler->getPlayerInput(0) == QUIT)
//             break;
//         else if (_graphicHandler->getPlayerInput(0) >= SWAP_LIBNCURSES &&
//                  _graphicHandler->getPlayerInput(0) <= SWAP_LIBRAYLIB)
//         {
//             if (handleLibSwitch())
//                 break;
//         }
//         else
//         {
//             bufferSend += std::to_string(_graphicHandler->getPlayerInput(0));
//             byteSent = send(_fd, bufferSend.c_str(), 2, 0);
//             if (byteSent < 0)
//                 throw std::runtime_error("In Client::readData(): send() host is probably offline");
//             bufferSend.clear();
//         }
//     }
// }

// // Read first data sent by the host which are width height and a bool to know if sound is ON or not
// // Format received is "i:w h s"
// void Client::readInitData(std::string &buffer)
// {
//     buffer.erase(0, 2);
//     _width = std::stoi(buffer, nullptr, 10);
//     buffer.erase(0, 2);
//     _height = std::stoi(buffer, nullptr, 10);
//     buffer.erase(0, 3);
//     _isSound = std::stoi(buffer, nullptr, 10);
// }

// // Read game data then draw
// // Format received is "pid+1:x y|x y|x y|\n(pid-1:x y|x y|x y|)\nf:x y"
// // i is player index, d is direction from -2 to 2 without 0
// void Client::readGameData(std::string &buffer)
// {
//     bool isP0 = true;
//     std::deque<point_t> vecPlayer0Body;
//     int player0Dir;
//     std::deque<point_t> vecPlayer1Body;
//     int player1Dir;
//     buffer.erase(0, 2);
//     int i;
//     for (i = 0; buffer[i]; i++)
//     {
//         if (buffer[i] == 'p')
//         {
//             isP0 = false;
//             i += 2;
//             if (i > (int)buffer.size())
//                 break;
//         }
//         if (isP0 && buffer[i] == 'd' && buffer[i + 1])
//         {
//             player0Dir = std::stoi(&buffer[i + 1], 0, 10);
//             i += 3;
//             if (i > (int)buffer.size())
//                 break;
//         }

//         if (!isP0 && buffer[i] == 'd' && buffer[i + 1])
//         {
//             player1Dir = std::stoi(&buffer[i + 1], 0, 10);
//             i += 3;
//             if (i > (int)buffer.size())
//                 break;
//         }

//         if (isdigit(buffer[i]))
//         {
//             point_t bodyPart;
//             bodyPart.x = std::stoi(&buffer[i], 0, 10);
//             i += 2;
//             if (i > (int)buffer.size())
//                 break;
//             bodyPart.y = std::stoi(&buffer[i], 0, 10);
//             i += 2;
//             if (isP0)
//                 vecPlayer0Body.push_back(bodyPart);
//             else
//                 vecPlayer1Body.push_back(bodyPart);
//             if (i > (int)buffer.size())
//                 break;
//         }

//         if (buffer[i] == 'f')
//             break;
//     }
//     buffer.erase(0, i + 2);
//     point_t foodPoint;
//     foodPoint.x = std::stoi(buffer, 0, 10);
//     buffer.erase(0, 3);
//     foodPoint.y = std::stoi(buffer, 0, 10);
//     _graphicHandler->clearBoard();
//     _graphicHandler->drawPlayer(Player(0, vecPlayer0Body, player0Dir));
//     _graphicHandler->drawPlayer(Player(1, vecPlayer1Body, player1Dir));
//     _graphicHandler->drawFood(foodPoint);
// }

// // Read Game Over message and print it in terminal
// // Format is "m: xxx"
// void Client::readMessage(std::string &buffer)
// {
//     buffer.erase(0, 2);
//     std::cout << buffer << std::endl;
// }

// int Client::handleLibSwitch()
// {
//     try
//     {
//         // if (_graphicHandler->getPlayerInput(0) == SWAP_LIBNCURSES)
//         //     _graphicHandler = _libHandler->switchGraphicLib(LibHandler::LIBNCURSES, std::move(_graphicHandler));
//         // else if (_graphicHandler->getPlayerInput(0) == SWAP_LIBSDL)
//         //     _graphicHandler = _libHandler->switchGraphicLib(LibHandler::LIBSDL, std::move(_graphicHandler));
//         // else if (_graphicHandler->getPlayerInput(0) == SWAP_LIBRAYLIB)
//         //     _graphicHandler = _libHandler->switchGraphicLib((LibHandler::lib_graphic_e)(SWAP_LIBRAYLIB - 3),
//         //                                                     std::move(_graphicHandler));
//         _graphicHandler->resetPlayerInput();
//         return 0;
//     }
//     catch (const std::exception &e)
//     {
//         std::cerr << "In Game: handleLibSwitch(): " << std::endl;
//         return 1;
//     }
// }

// Client::~Client()
// {
//     close(_fd);
//     // _libHandler->destroyGraphicLib(std::move(_graphicHandler));
//     // if (_isSound)
//     // _libHandler->destroySoundLib(std::move(_soundHandler));
// }