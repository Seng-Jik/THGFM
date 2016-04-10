#pragma once
#include <iostream>
#include "GameDataMgr.h"

using namespace std;

GameDataMgr gameData;

void GameDataMgr::ReadInitSetting(Uint8* gamepad){
    ifstream in("PAW01.sav",ios::binary);
    
    in.read((char*)gamepad,8);
    
    Uint64 bools;
    in.read((char*)&bools,8);
    //m_bools.assign(bools);
    
    in.close();
}

void GameDataMgr::SetGamePad(const Uint8* gamepad){
    fstream f("PAW01.sav",ios::binary|ios::out|ios::app|ios::beg);
    //f.write(gamepad,8);
    f.close();
}

void GameDataMgr::Flush(){
    fstream f("PAW01.sav",ios::binary|ios::out|ios::app);
    
	f.seekp(4, ios::beg);
    //auto bools = m_bools.to_ulong();
    //f.write((char*)&bools,8);
    
    f.close();
}