#include "Snow.h"
#include "KeyMapAct.h"
#include "WSTGame.h"
#include <iostream>
#include "Tools.h"
#include "BulletMgr.h"
using namespace std;
using namespace Snow;

int main(int argc,char** argv){
    Init();
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"2");
    pRnd.Create("东方谷丰梦",false,WIDTH,HEIGHT);

    Player::Init();
    StageMgr::Init();
    BulletMgr::Init();
    Run(new WSTGame);

    return 0;
}
