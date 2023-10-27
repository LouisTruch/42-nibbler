#!/bin/bash



# SFML
# Check if file is already here
wget https://www.sfml-dev.org/files/SFML-2.6.0-linux-gcc-64-bit.tar.gz
mkdir -p libs/sfml/lib && tar xf SFML-2.6.0-linux-gcc-64-bit.tar.gz -C libs/sfml/lib --strip-components 1
rm -rf SFML-2.6.0-linux-gcc-64-bit.tar.gz
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${PWD}/libs/sfml/lib"

# Download MLX42
# git clone https://github.com/codam-coding-college/MLX42.git libs/mlx/lib/

# mkdir libs/mlx/lib/build
# cmake libs/mlx/lib -B libs/mlx/lib/build

# make -sC libs/mlx/lib/build