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
using namespace std;
using namespace Snow;
Snow::Mutex initMutex;

void _initThread(THREAD_ID){
    initMutex.Lock();
    LoadStage("Stage1",LV_N);
    initMutex.Unlock();
    PNT("INIT END");
}
static Snow::Thread initThread(&_initThread);

int main(int argc,char** argv){
    Init();
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"2");
    SDL_SetHint(SDL_HINT_VIDEO_ALLOW_SCREENSAVER,"0");
    SDL_SetHint(SDL_HINT_RENDER_DIRECT3D_THREADSAFE,"1");
    pRnd.Create("东方谷丰梦",FALSE,WIDTH,HEIGHT);
    SDL_ShowCursor(0);

    ACGCross::Logo* acgclogo = new ACGCross::Logo;
    KeyMapAct::Init();
    Player::Init();
    PlayerBullet::Init();
    BulletMgr::Init();
    EffectMgr::Init();
    SeMgr::Init();
    ItemMgr::Init();
    pause = new PauseActivity;
    bossConversation = new BossConversation;
    wstg = new WSTGame;
    acgclogo -> SetGoto(*new TouHouGameLogo);
    StageMgr::Init();
    marisa.Init();
    reimu.Init();
    gameUI.Init();

    #ifdef _DEBUG
    acgclogo ->SetInitThread(&initThread);
    Run(acgclogo);
    #else
    _initThread(nullptr);
    Run(wstg);
    #endif
    return 0;
}
