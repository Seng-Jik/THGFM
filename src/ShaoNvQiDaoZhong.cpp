#include "ShaoNvQiDaoZhong.h"
#include "Tools.h"
using namespace std;

SDL_Texture* ShaoNvQiDaoZhong::m_ani[89];

void ShaoNvQiDaoZhong::Init()
{
    string image;
    for(int i = BEG_BEG;i <= END_END;++i){
        image = "GameUI/Loading/snqdz00";
        if(i<=9) image += '0';
        image += to_string(i);
        image += ".png";
        m_ani[i] = LoadPic(image.c_str());
    }
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
    SDL_RenderCopy(Snow::pRnd,m_ani[m_frame],nullptr,&DSTRECT);
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

