#include "TouHouGameLogo.h"
#include "WSTGame.h"
#include "Tools.h"

TouHouGameLogo* touHouGameLogo;

TouHouGameLogo::TouHouGameLogo()
{
    SetLogicScreenSize(WIDTH,HEIGHT);
    //ctor
}

TouHouGameLogo::~TouHouGameLogo()
{
    //dtor
}

void TouHouGameLogo::OnNext()
{
    if(m_tmr.GetTimer() == 179){
        m_rightTop.SetAlpha(0);
        m_center.SetAlpha(0);
        m_rightBottom.SetAlpha(0);
    }
    else if(m_tmr.GetTimer()>=180){
        Snow::Goto(wstg);
    }
    else if(m_tmr.GetTimer()<=15){
        float per = float(m_tmr.GetTimer())/15;
        m_rightTop.SetAlpha(per*255);
        m_center.SetAlpha(per*255);
        m_rightBottom.SetAlpha(per*255);
    }else if(m_tmr.GetTimer()>=165){
        float per = float(m_tmr.GetTimer()-165)/15;
        m_rightTop.SetAlpha(255-per*255);
        m_center.SetAlpha(255-per*255);
        m_rightBottom.SetAlpha(255-per*255);
    }
}

void TouHouGameLogo::OnDraw()
{
    SDL_SetRenderDrawColor(Snow::pRnd,255,255,255,255);
    Snow::pRnd.Clear();
    m_center.OnDraw();
    m_rightTop.OnDraw();
    m_rightBottom.OnDraw();
}

void TouHouGameLogo::OnHide()
{
    delete this;
}
#include "Tools.h"
void TouHouGameLogo::OnShow()
{
    m_center.Load("Title/Center.png");
    m_rightBottom.Load("Title/RightBottom.png");
    m_rightTop.Load("Title/RightTop.png");
    m_center.SetPos(int(WIDTH/2-350),int(HEIGHT/2-33));
    m_rightBottom.SetPos(WIDTH-516,HEIGHT-62);
    m_rightTop.SetPos(WIDTH-500,0);
    m_rightTop.SetBlend(SDL_BLENDMODE_BLEND);

    m_tmr.Reset();
}
