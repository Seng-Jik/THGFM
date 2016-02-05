#pragma once
#include "Snow.h"
#include <string>
#define PI 3.1415926535897932384626433832795
#define Ang2Arc(x) (x)/180*PI
#define WIDTH 1280
#define HEIGHT 720
enum Level{
    LV_E,LV_N,LV_H,LV_L,LV_EX
};

SDL_Texture* LoadPic(const std::string&);

void LoadStage(const std::string&,Level);
