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
using namespace std;
using namespace Snow;

int main(int argc,char** argv){
    Init();
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"2");
    pRnd.Create("东方谷丰梦",FALSE,WIDTH,HEIGHT);

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
    StageMgr::Init();
    marisa.Init();
    reimu.Init();
    gameUI.Init();
    Run(wstg);

    return 0;
}
