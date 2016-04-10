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

#include <float.h>
unsigned int fp_control_state = _controlfp(_EM_INEXACT, _MCW_EM);

using namespace std;
using namespace Snow;
using namespace ACGCross;

Snow::Mutex initMutex;

void InitParttens(){
    extern void InitScPartten_1L();
    InitScPartten_1L();

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

    wstg = new WSTGame;

    StageMgr::Init();
    marisa.Init();
    reimu.Init();
    gameUI.Init();
    scClock.Init();
}

extern Uint8 BetaStart();
extern void SoundFinished(int channel);
extern void InitScBgs();

int main(int argc,char** argv){
    Init();
  
    Mix_ChannelFinished(&SoundFinished);

    InitParttens();
    
    InitScBgs();

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"2");
    SDL_SetHint(SDL_HINT_VIDEO_ALLOW_SCREENSAVER,"0");
    SDL_SetHint(SDL_HINT_RENDER_DIRECT3D_THREADSAFE,"1");
    SDL_SetHint(SDL_HINT_RENDER_DIRECT3D11_DEBUG,"0");

	pRnd.Create("THGFM", false, 1280, 720);

	Logo* acgclogo=nullptr;

    InitGameLogic(acgclogo);

	BetaStart();

    Run(new ShaoNvQiDaoZhong(new std::thread(&_initThread,nullptr),wstg));

    return 0;
}

