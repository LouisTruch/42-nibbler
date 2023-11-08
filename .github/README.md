# **Nibbler-42**

This 42 school projet aims to get a better understanding of shared objects. It's about coding a Snake-like game in C++ and using dlopen() C syscall's to dynamically load graphic library on the go. We must have at least 3 different graphic libraries.

I used c++20 to build this project and the 3 libraries I built my shared objects with are : 
- [**Ncurses**](https://github.com/mirror/ncurses) (terminal based)
- [**SDL2**](https://github.com/libsdl-org/SDL)
- [**Raylib**](https://github.com/raysan5/raylib)

Bonus for nibbler are as followed : adding more game modes, adding a sound library (still compiled in a shared object and opened in game), add a local multiplayer and finally add a network based multiplayer.

## Ncurses
![screenshot of a Snake game rendered with Ncurses](https://github.com/LouisTruch/42-nibbler/blob/master/.github/ncurses.png)

## SDL2
![screenshot of a Snake game rendered with SDL2](https://github.com/LouisTruch/42-nibbler/blob/master/.github/sdl.png)

## Raylib
![screenshot of a Snake game rendered with Raylib](https://github.com/LouisTruch/42-nibbler/blob/master/.github/raylib.png)

## Bonus

I made a little menu (with Ncurses) to choose from every single option. <br />
![screenshot of the menu for the Snake game](https://github.com/LouisTruch/42-nibbler/blob/master/.github/menu.png)
Bonus I made are :
- Changing Speed : Every time a player eats the game speeds up
- Disappearing Food : Food teleports every x seconds
- Hunger : Player dies if he does not eat every x seconds
- Score (single player only) : High score for a given map size are saved in a file and updated if a player beats it. Current score and high score are also printed in the game
- Sound : Used Raylib to play a sound when a player eats
- Local multiplayer : 2 players on the same keyboard
- Network multiplayer : First player host and the second must join using first player local IP 
<br />

## **Run**
**You need glfw3**<br />
Git clone the repo <br />
Use "make install" <br />
Use "./nibbler" <br />

## Assets
The assets used in Raylib are from [**here**](https://opengameart.org/content/snake-game-assets).