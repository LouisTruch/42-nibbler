#/bin/bash

sdl2_config=$(pkg-config --cflags --libs sdl2)

sdl2_repo="https://github.com/libsdl-org/SDL.git -b SDL2"

if [ $? -eq 0 ]; then
    echo "pck-config successful."
    if [ -n "$sdl2_config" ]; then
        echo "SDL2 found. SDL2 configuration: $sdl2_config"
        exit 0
    else
        echo "SDL2 not found. Cloning and building SDL2 repository..."
        git clone $sdl2_repo ./libs/sdl/SDL
        cd ./libs/sdl/SDL
        mkdir build
        cd build
        ../configure
        make
        exit 1
    fi
else
    echo "pkg-config failed. Please check SDL2 installation."
    exit 2
fi