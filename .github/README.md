# Nibbler-42

This 42 school projet aims to get a better understanding of shared objects. It's about coding a Snake-like game in C++ and using dlopen() C syscall's to dynamically load graphic library on the go. We must have at least 3 different graphic libraries.

I used c++20 to build this project and the 3 libraries are : 
- ncurses (terminal based)
- SDL2
- raylib

Bonus for nibbler are as followed : adding more game modes, adding a sound library (still compiled in a shared object and opened in game), add a local multiplayer and finally add a network based multiplayer.

# Ncurses
![screenshot of a Snake game rendered with Ncurses](https://github.com/LouisTruch/42-nibbler/blob/master/.github/ncurses.png)

# SDL2
![screenshot of a Snake game rendered with SDL2](https://github.com/LouisTruch/42-nibbler/blob/master/.github/sdl.png)

# Raylib
![screenshot of a Snake game rendered with Raylib](https://github.com/LouisTruch/42-nibbler/blob/master/.github/raylib.png)

# Bonus
I made a little menu (with Ncurses) to choose from every single option.
![screenshot of the menu for the Snake game](https://github.com/LouisTruch/42-nibbler/blob/master/.github/menu.png)

# Run
Git clone the repo
Use "make install"
use "./nibbler"