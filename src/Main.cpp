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
#include "Title.h"
#include "SCClock.h"
#include "BasicPackReader.h"
#include "GameDataMgr.h"
#include "ShaoNvQiDaoZhong.h"
#include "Title.h"
using namespace std;
using namespace Snow;
Snow::Mutex initMutex;

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
    //Run(new Title);
    #endif
    return 0;
}
