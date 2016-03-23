#include "Snow.h"
#include "KeyMapAct.h"
#include "WSTGame.h"
#include <iostream>
#include "Tools.h"
#include "BulletMgr.h"
#include "EffectMgr.h"
#include "PlayerBullet.h"
#include "SeMgr.h"
#include "PauseActivity.h"
#include "ItemMgr.h"
#include "BossConversation.h"
#include "CharData.h"
#include "GameUI.h"
#include "ACGCross_Logo.h"
#include "Snow/Debug.h"
#include "TouHouGameLogo.h"
#include "Title/TitleMenu.h"
#include "SCClock.h"
#include "GameDataMgr.h"
#include "ACPReader.h"
#include "ShaoNvQiDaoZhong.h"
using namespace std;
using namespace Snow;
Snow::Mutex initMutex;

void InitParttens(){
    extern void InitScPartten_1L();
    InitScPartten_1L();
    extern void InitShtPartten_1L();
    InitShtPartten_1L();

    extern void InitScBgs();
    InitScBgs();
}

void _initThread(THREAD_ID){
    initMutex.Lock();
    LoadStage("Stage1",LV_L);
    initMutex.Unlock();
    PNT("INIT END");
}
static Snow::Thread initThread(&_initThread);

static void InitGameLogic(ACGCross::Logo* acgclogo){
    Uint8 gamePadSetting[8];
    //gameDataMgr.ReadInitSetting(gamePadSetting);
    KeyMapAct::Init();
    //TODO:KeyMapAct::LoadSetting
    Player::Init();
    ShaoNvQiDaoZhong::Init();
    PlayerBullet::Init();
    BulletMgr::Init();
    EffectMgr::Init();
    SeMgr::Init();
    ItemMgr::Init();
    pause = new PauseActivity;
    //bossConversation = new BossConversation;
    wstg = new WSTGame;
    acgclogo -> SetGoto(*new TouHouGameLogo);
    StageMgr::Init();
    marisa.Init();
    reimu.Init();
    gameUI.Init();
    scClock.Init();
}

int main(int argc,char** argv){
    Init();
    extern Uint8 BetaStart();
    //BetaStart();

    extern void SoundFinished(int channel);
    Mix_ChannelFinished(&SoundFinished);

    //ACPReader acp;
    //acp.OpenPkg("assets.acp");
    //ResFile::InstallReader(&acp);

    //BasicPackReader bks[3];
    //bks[0].OpenPkg("base.bpk");
    //bks[1].OpenPkg("graphics.bpk");
    //bks[2].OpenPkg("stage1.bpk");
    //for(int i = 0;i < 3;++i) Snow::ResFile::InstallReader(&bks[i]);

    InitParttens();
    extern void InitScBgs();
    InitScBgs();

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"2");
    SDL_SetHint(SDL_HINT_VIDEO_ALLOW_SCREENSAVER,"0");
    SDL_SetHint(SDL_HINT_RENDER_DIRECT3D_THREADSAFE,"1");
    SDL_SetHint(SDL_HINT_RENDER_DIRECT3D11_DEBUG,"0");

    //BasicPackReader data;
    //data.OpenPkg("data.+pk");
    //Snow::ResFile::InstallReader(&data);
    pRnd.Create("东方谷丰梦",FALSE,800,450);

    ACGCross::Logo* acgclogo = new ACGCross::Logo;
    InitGameLogic(acgclogo);

    #ifndef _DEBUG
    SDL_ShowCursor(0);
    acgclogo ->SetInitThread(&initThread);
    Run(acgclogo);
    #else
    //_initThread(nullptr);
    Run(new ShaoNvQiDaoZhong(new std::thread(&_initThread,nullptr),wstg));
    //Run(&titleMenu);
    #endif
    exit(0);
    return 0;
}
/*
#include "lua.hpp"
int main(int argc,char** argv){
    Init();
    Snow::ResFile r("EnemyParttens/HelloEnemy.lua");
    auto L = luaL_newstate();
    luaL_openlibs(L);
    luaL_loadbuffer(L,r,r.Size(),"line");
    int i = lua_pcall(L,0,0,0);
    PNT(i);
    system("pause");
}*/
