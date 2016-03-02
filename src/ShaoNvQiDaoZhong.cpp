#include "ShaoNvQiDaoZhong.h"
#include "Snow/Debug.h"
#include "Tools.h"
using namespace std;
std::atomic_bool ShaoNvQiDaoZhong::m_continueRun;
SDL_Texture* ShaoNvQiDaoZhong::m_ani;

void ShaoNvQiDaoZhong::Init()
{
    m_ani = LoadPic("GameUI/Loading.png");
}
void ShaoNvQiDaoZhong::EndShaoNv()
{
    m_continueRun = false;
}

void ShaoNvQiDaoZhong::OnShow()
{
    m_continueRun = true;
}

ShaoNvQiDaoZhong::ShaoNvQiDaoZhong(std::thread* t,Snow::Activity* c)
{
    m_frame = BEG_BEG;
    m_act = c;
    m_thd = t;
    SetLogicScreenSize(1280,720);
}

void ShaoNvQiDaoZhong::OnHide()
{
    if(m_thd->joinable()) m_thd -> join();
    delete m_thd;
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
    //PNT(m_thd->joinable());
    if(m_frame == LOOP_END+1){
        if(m_continueRun) m_frame = LOOP_BEG;
    }
    else if(m_frame == END_END){
        if(m_act != nullptr) Snow::Goto(m_act);
        else Snow::Return();
    }
}

