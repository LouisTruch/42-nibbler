#/bin/bash

sdl2_config=$(pkg-config --cflags --libs sdl2)

sdl2_repo="https://github.com/libsdl-org/SDL.git -b SDL2"

# Check if pkg-config was successful
echo "$sdl2_config"
if [ $? -eq 0 ]; then
    echo "pck-config successful."
#     # Check if sdl2_config is not empty
    if [ -n "$sdl2_config" ]; then
        echo "SDL2 found. SDL2 configuration: $sdl2_config"
        exit 0
    else
        git clone $sdl2_repo
        cd SDL
        mkdir build
        cd build
        ../configure
        make
        echo $sdl2_repo
    exit 1
    fi
else
    echo "pkg-config failed. Please check SDL2 installation."
    exit 2
fi