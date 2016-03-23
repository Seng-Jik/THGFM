#pragma once
#include "Snow.h"
#include <string>
#define PI 3.1415926535897932384626433832795
#define Ang2Arc(x) (x)/180*PI
#define WIDTH 1280
#define HEIGHT 720
enum Level{
    LV_E=0,
    LV_N=1,
    LV_H=2,
    LV_L=3,
    LV_EX=4
};

SDL_Texture* LoadPic(const std::string&);

void LoadStage(const std::string&,Level);
