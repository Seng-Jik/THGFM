#pragma once
#include <cstdio>
#include "GameDataMgr.h"

#define OPEN_SAVE_W FILE* f = fopen("PAW02.sav","wb")
#define OPEN_SAVE_R FILE* f = fopen("PAW02.sav","rb")
#define CLOSE_SAVE fclose(f)

GameDataMgr gameData;

void GameDataMgr::ReadInitSetting(Uint8* gamepad){
    OPEN_SAVE_R;
    fread(f,gamepad,8);
    CLOSE_SAVE;
}

void GameDataMgr::SetGamePad(const Uint8* gamepad){
    OPEN_SAVE_W;
    fwrite(f,gamepad,8);
    CLOSE_SAVE;
}