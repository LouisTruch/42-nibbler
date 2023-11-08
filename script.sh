#!/bin/bash

#Download raylib repo and build library
git clone https://github.com/raysan5/raylib.git ./libs/raylib/raylib
cd ./libs/raylib/raylib/src
make