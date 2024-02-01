#!/bin/bash

raylib_path="./libs/raylib/raylib"

if [ -e $raylib_path ]; then
    echo "raylib found."
    make -C $raylib_path/src
    exit 0
else
    #Download raylib repo and build it
    echo "raylib not found. Cloning and building raylib repository..."
    git clone https://github.com/raysan5/raylib.git $raylib_path
    make -C $raylib_path/src
    exit 1
fi