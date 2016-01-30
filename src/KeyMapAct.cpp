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

    else if(e.type == SDL_JOYAXISMOTION){
        int& oldValue = m_axis[e.jaxis.which][e.jaxis.axis];

        if(e.jaxis.axis == 1){  //竖直方向
            cout<<"H"<<e.jaxis.value<<endl;

            if(oldValue < 0 && e.jaxis.value >=0) OnEvent(e.jaxis.which,T_UP,false);
            if(oldValue <=0 && e.jaxis.value > 0) OnEvent(e.jaxis.which,T_DOWN,true);

            if(oldValue > 0 && e.jaxis.value<=0) OnEvent(e.jaxis.which,T_DOWN,false);
            if(oldValue >= 0 && e.jaxis.value <0) OnEvent(e.jaxis.which,T_UP,true);
            oldValue = e.jaxis.value;
        }

        else if(e.jaxis.axis == 0){  //水平方向
            cout<<"H"<<e.jaxis.value<<endl;

            if(oldValue < 0 && e.jaxis.value >=0) OnEvent(e.jaxis.which,T_LEFT,false);
            if(oldValue <=0 && e.jaxis.value > 0) OnEvent(e.jaxis.which,T_RIGHT,true);

            if(oldValue > 0 && e.jaxis.value<=0) OnEvent(e.jaxis.which,T_RIGHT,false);
            if(oldValue >= 0 && e.jaxis.value <0) OnEvent(e.jaxis.which,T_LEFT,true);
            oldValue = e.jaxis.value;
        }

    }else if(e.type == SDL_JOYBUTTONDOWN){
        Key k = m_k[e.jbutton.which] [e.jbutton.button];
        if(k != T_NONE) OnEvent(int(e.jbutton.which),k,true);
    }else if(e.type == SDL_JOYBUTTONUP){
        Key k = m_k[e.jbutton.which] [e.jbutton.button];
        if(k != T_NONE) OnEvent(int(e.jbutton.which),k,false);
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
int KeyMapAct::m_axis [2][2];
void KeyMapAct::Init()
{
    for(int i = 0;i < SDL_NumJoysticks();++i)
        SDL_JoystickOpen(i);
    LoadConfig();
    for(int i = 0;i<2;++i)
        for(int j = 0;j<2;++j)
            m_axis[i][j] = 0;
}

