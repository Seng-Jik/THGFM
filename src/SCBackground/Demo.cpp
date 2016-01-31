<<<<<<< HEAD
#include "Snow.h"
#include "Snow/Debug.h"
using namespace Snow;
typedef void(*SCBg)(int cnt);

void SCBG001(int cnt){
    SDL_SetRenderDrawColor(pRnd,0,0,255,255);
    SDL_RenderClear(pRnd);
}

SCBg scbgs [] = {
    &SCBG001
};
=======
#include "Snow.h"
#include "Snow/Debug.h"
using namespace Snow;
typedef void(*SCBg)(int cnt);

void SCBG001(int cnt){
    SDL_SetRenderDrawColor(pRnd,0,0,255,255);
    SDL_RenderClear(pRnd);
}

SCBg scbgs [] = {
    &SCBG001
};
>>>>>>> 720c0b59a97be74f02a7c85da646f3f62bece3a5
