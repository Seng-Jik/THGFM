#include "KeyMapAct.h"
#include "Snow/ResVal.h"
#include "Snow/Debug.h"
#include "Tools.h"
using namespace Snow;
using namespace std;

Key KeyMapAct::m_k [2] [32];

KeyMapAct::KeyMapAct()
{
    SetLogicScreenSize(WIDTH,HEIGHT);
    LoadConfig();
    RefreshGamePadList();
}

void KeyMapAct::LoadConfig()
{
    ResVal r;r.Load("keyCfg.rv");

    for(int i = 0;i < 2;++i)
        for(int j = 0;j < 32;++j)
            m_k [i] [j] = T_NONE;

    //1P 方案
    m_k [0] [r.Int("P1SHOT")] = T_SHOT;
    m_k [0] [r.Int("P1BOOM")] = T_BOOM;
    m_k [0] [r.Int("P1SLOW")] = T_SLOW;
    m_k [0] [r.Int("P1PAUSE")] = T_PAUSE;

    //2P 方案
    m_k [1] [r.Int("P2SHOT")] = T_SHOT;
    m_k [1] [r.Int("P2BOOM")] = T_BOOM;
    m_k [1] [r.Int("P2SLOW")] = T_SLOW;
    m_k [1] [r.Int("P2PAUSE")] = T_PAUSE;
}

void KeyMapAct::OnEvent(const SDL_Event& e)
{
    Key k;

    if(e.type == SDL_KEYDOWN){
        k = kb2k(e.key.keysym.sym);
        if(k != T_NONE) OnEvent(0,k,true);

    }else if(e.type == SDL_KEYUP){
        k = kb2k(e.key.keysym.sym);
        if(k != T_NONE) OnEvent(0,k,false);
    }

}

Key KeyMapAct::kb2k(SDL_Keycode c)
{
    switch(c){
    case SDLK_ESCAPE:
        return T_ESC;
    case 13:
        return T_ENTER;
    case SDLK_z:
        return T_SHOT;
    case SDLK_x:
        return T_BOOM;
    case SDLK_LSHIFT:
        return T_SLOW;
    case SDLK_UP:
        return T_UP;
    case SDLK_DOWN:
        return T_DOWN;
    case SDLK_LEFT:
        return T_LEFT;
    case SDLK_RIGHT:
        return T_RIGHT;
    case SDLK_SPACE:
        return T_PAUSE;
    default:
        return T_NONE;
    }
}

void KeyMapAct::RefreshGamePadList()
{

}

int KeyMapAct::GetLinkedGamePadCount()
{
    return SDL_NumJoysticks();
}
