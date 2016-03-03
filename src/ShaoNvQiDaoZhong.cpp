#include "ShaoNvQiDaoZhong.h"
#include "Snow/Debug.h"
#include "Tools.h"
using namespace std;
std::atomic_bool ShaoNvQiDaoZhong::m_continueRun;
SDL_Texture* ShaoNvQiDaoZhong::m_ani1,*ShaoNvQiDaoZhong::m_ani2;

void ShaoNvQiDaoZhong::Init()
{
    m_ani1 = LoadPic("GameUI/ShaoNvQiDaoZhong1.png");
    m_ani2 = LoadPic("GameUI/ShaoNvQiDaoZhong2.png");
}
void ShaoNvQiDaoZhong::EndShaoNv()
{
    m_continueRun = false;
}

void ShaoNvQiDaoZhong::OnShow()
{
    m_continueRun = true;
    m_yinyang_angle = 0;
    m_state = FADEIN;
    m_yinyang_alpha = 0;
    m_qidao_alpha = 0;
    m_qidao_alpha_down = true;
}

ShaoNvQiDaoZhong::ShaoNvQiDaoZhong(std::thread* t,Snow::Activity* c)
{
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
    const SDL_Rect dst1={998,640,48,48};
    const SDL_Point cnt = {23,23};
    SDL_RenderCopyEx(Snow::pRnd,m_ani1,nullptr,&dst1,m_yinyang_angle,&cnt,SDL_FLIP_NONE);
    const SDL_Rect dst2={1066,653,182,21};
    SDL_RenderCopy(Snow::pRnd,m_ani2,nullptr,&dst2);
}

void ShaoNvQiDaoZhong::OnNext()
{
    if(!m_continueRun) m_state = FADEOUT;
    if(m_state == FADEIN){
        if(m_qidao_alpha < 245) m_qidao_alpha+=10;
        if(m_qidao_alpha >= 245) m_state = RUNNING;
        m_yinyang_alpha = m_qidao_alpha;
    }
    else if(m_state == FADEOUT){
        if(m_qidao_alpha > 10) m_qidao_alpha-=10;
        if(m_yinyang_alpha >= 10) m_yinyang_alpha-=10;
        if( m_yinyang_alpha < 10){
            if(m_act) Snow::Goto(m_act);
            else Snow::Return();
            return;
        }
    }
    if(m_state == RUNNING){
        if(m_qidao_alpha_down){
            m_qidao_alpha -= 10;
            if(m_qidao_alpha<50) m_qidao_alpha_down = false;
        }
        else{
            m_qidao_alpha +=10;
            if(m_qidao_alpha>245) m_qidao_alpha_down = true;
        }
    }
    SDL_SetTextureAlphaMod(m_ani1,m_yinyang_alpha);
    SDL_SetTextureAlphaMod(m_ani2,m_qidao_alpha);
    m_yinyang_angle += 10;
}

