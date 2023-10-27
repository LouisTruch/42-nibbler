#include "../inc/MlxGraphicLib.hpp"

MlxGraphicLib::MlxGraphicLib(int width, int height)
{
    _mlx = mlx_init(width * MLX_TILE_SIZE, height * MLX_TILE_SIZE, "MLX Nibbler", false);
    if (!_mlx)
    {
        // Handle error
    }
    _image = mlx_new_image(_mlx, width * MLX_TILE_SIZE, height * MLX_TILE_SIZE);
    if (!_image)
    {

    }

}

void MlxGraphicLib::drawPlayer(const body_t &body)
{
    (void)body;
}

void MlxGraphicLib::drawFood(const point_t &point)
{
    (void)point;
}

void MlxGraphicLib::registerPlayerInput()
{
}

player_input_t MlxGraphicLib::getPlayerInput() const
{
    return _playerInput;
}

void MlxGraphicLib::resetPlayerInput()
{
    _playerInput = DEFAULT;
}

MlxGraphicLib::MlxGraphicLib()
{
}

MlxGraphicLib::~MlxGraphicLib()
{
    mlx_terminate(_mlx);
}

MlxGraphicLib::MlxGraphicLib(const MlxGraphicLib &other)
{
    *this = other;
}

MlxGraphicLib &MlxGraphicLib::operator=(const MlxGraphicLib &other)
{
    if (&other == this)
        return *this;
    return *this;
}

std::unique_ptr<MlxGraphicLib> makeGraphicLib(int width, int height)
{
    return std::make_unique<MlxGraphicLib>(width, height);
}

void destroyGraphicLib(std::unique_ptr<MlxGraphicLib> gLib)
{
    gLib.reset();
}