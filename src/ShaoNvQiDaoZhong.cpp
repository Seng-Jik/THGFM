#include "ShaoNvQiDaoZhong.h"
#include "Tools.h"
using namespace std;

SDL_Texture* ShaoNvQiDaoZhong::m_ani;

void ShaoNvQiDaoZhong::Init()
{
    m_ani = LoadPic("GameUI/Loading.png");
}


ShaoNvQiDaoZhong::ShaoNvQiDaoZhong(void(*func)(Snow::THREAD_ID),Snow::Activity* c):m_thd(func)
{
    m_frame = BEG_BEG;
    m_act = c;
}

void ShaoNvQiDaoZhong::OnHide()
{
    delete this;
}

void ShaoNvQiDaoZhong::OnDraw()
{
    SDL_Rect src = {0,58*m_frame,384,58};
    SDL_RenderCopy(Snow::pRnd,m_ani,&src,&DSTRECT);
}

void ShaoNvQiDaoZhong::OnNext()
{
    m_frame++;
    if(m_frame == LOOP_END+1){
        if(m_thd.Running()) m_frame = LOOP_BEG;
    }
    else if(m_frame == END_END){
        if(m_act) Snow::Goto(m_act);
        else Snow::Return();
    }
}

void ShaoNvQiDaoZhong::SendMsg(int i)
{
    m_thd.SendMsg(i);
}

