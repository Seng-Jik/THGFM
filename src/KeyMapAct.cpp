#include "KeyMapAct.h"
#include "Snow/ResVal.h"
#include "Snow/Debug.h"
using namespace Snow;
using namespace std;

Key KeyMapAct::m_k [2] [32];
int KeyMapAct::m_devs [2];
SDL_Joystick* KeyMapAct::m_gamepads [2];
SDL_JoystickID KeyMapAct::m_gid [2];

KeyMapAct::KeyMapAct()
{
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
    m_devs[0] = r.Int("P1DEVICE");
    if(m_devs[0]!=0){
        m_k [0] [r.Int("P1SHOT")] = T_SHOT;
        m_k [0] [r.Int("P1BOOM")] = T_BOOM;
        m_k [0] [r.Int("P1SLOW")] = T_SLOW;
        m_k [0] [r.Int("P1PAUSE")] = T_PAUSE;
    }

    //2P 方案
    m_devs[1] = r.Int("P2DEVICE");
    if(m_devs[1]!=0){
        m_k [1] [r.Int("P2SHOT")] = T_SHOT;
        m_k [1] [r.Int("P2BOOM")] = T_BOOM;
        m_k [1] [r.Int("P2SLOW")] = T_SLOW;
        m_k [1] [r.Int("P2PAUSE")] = T_PAUSE;
    }
}

void KeyMapAct::OnEvent(const SDL_Event& e)
{
    int p;
    Key k;
    bool s;

    if(e.type == SDL_KEYDOWN){
        s = true;
        if(m_devs[0] == 0) p = 1;
        if(m_devs[1] == 0) p = 2;
        k = kb2k(e.key.keysym.sym);

        if(k != T_NONE) OnEvent(p,k,s);

    }else if(e.type == SDL_KEYUP){
        s = false;
        if(m_devs[0] == 0) p = 1;
        if(m_devs[1] == 0) p = 2;
        k = kb2k(e.key.keysym.sym);

        if(k != T_NONE) OnEvent(p,k,s);
    }

    else if(e.type == SDL_JOYAXISMOTION){
        p = gk2p(e.jaxis.which);
        OnEvent(p,k,s);
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

int KeyMapAct::gk2p(SDL_JoystickID id)
{
    for(int i = 0;i < 2;++i){
        if(m_devs[i] == 0) continue;
        else if(m_devs[i] == 1 && SDL_JoystickInstanceID(m_gamepads[0]) == id) return i+1;
        else if(m_devs[i] == 2 && SDL_JoystickInstanceID(m_gamepads[1]) == id) return i+1;
    }
    return 0;
}



void KeyMapAct::RefreshGamePadList()
{
    FOR_EACH(i,0,2){
        SDL_JoystickClose(m_gamepads[i]);
    }
    FOR_EACH(i,0,2){
        if(i<SDL_NumJoysticks()){
            m_gamepads[i] = SDL_JoystickOpen(i);
            m_gid[i] = SDL_JoystickInstanceID(m_gamepads[i]);
        }
    }

}

int KeyMapAct::GetLinkedGamePadCount()
{
    return SDL_NumJoysticks();
}
