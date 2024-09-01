#pragma once
#include <math.h>
#include <iostream>

constexpr float PI =3.14159;
constexpr float PI2 =PI/2;
constexpr float PI3 =3*PI/2;
constexpr float SPEED =0.1f;
constexpr float FOV =60.f;
constexpr int MAP_SIZE =20;
constexpr int TILE_SIZE =64;
constexpr int MINIMAP_SCALE =8;
constexpr int MINIMAP_TILE_SIZE = TILE_SIZE/MINIMAP_SCALE;
constexpr int WINDOW_WIDTH =1024;
constexpr int WINDOW_HEIGHT =512;
constexpr float dr =0.00174533/5.33333/2; //1 degree in radians divide by 10.6
constexpr float dr2 =0.0174533/5.33333; //1 degree in radians divide by 5.3