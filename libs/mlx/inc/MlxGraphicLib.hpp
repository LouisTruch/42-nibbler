#pragma once

#include "../../../inc/IGraphicLib.hpp"
#include "../lib/include/MLX42/MLX42.h"
// #include "../lib/include/MLX42/MLX42_Int.h"
#include <memory>

constexpr int MLX_TILE_SIZE = 5;

class MlxGraphicLib : public IGraphicLib
{
  public:
    MlxGraphicLib(int, int);
    ~MlxGraphicLib();
    MlxGraphicLib(const MlxGraphicLib &);
    MlxGraphicLib &operator=(const MlxGraphicLib &);
    player_input_t getPlayerInput() const;
    void resetPlayerInput();
    void registerPlayerInput();
    void drawPlayer(const body_t &);
    void drawFood(const point_t &);

  private:
    MlxGraphicLib();

  private:
    mlx_t *_mlx;
    mlx_image_t *_image;
};

extern "C"
{
    std::unique_ptr<MlxGraphicLib> makeGraphicLib(int width, int height);
    void destroyGraphicLib(std::unique_ptr<MlxGraphicLib> gLib);
}