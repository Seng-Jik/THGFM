#include "PauseActivity.h"
#include "WSTGame.h"
#include "Tools.h"
#include "Player.h"
PauseActivity* pause;
void PauseActivity::OnShow()
{
    SDL_Rect r = {0,0,Snow::pRnd.GetPhW(),Snow::pRnd.GetPhH()};
    Uint32* pixels = (new Uint32 [Snow::pRnd.GetPhW()*Snow::pRnd.GetPhH()]);
    SDL_RenderReadPixels(Snow::pRnd,
                     &r,
                     SDL_PIXELFORMAT_ARGB8888,
                     (void*)pixels,
                     Snow::pRnd.GetPhW()*4);
    m_bg.Load(SDL_CreateRGBSurfaceFrom((void*)pixels,Snow::pRnd.GetPhW(),Snow::pRnd.GetPhH(),32,Snow::pRnd.GetPhW()*4,
                                       0x00FF0000,0X0000FF00,0x000000FF,0XFF000000));
    m_bgt_o.Load(m_bg);
    m_bgt_o.SetPos(0,0);
    m_bgt_o.SetZoom(WIDTH,HEIGHT);
    Snow::FastBlurARGB8888(m_bg,40);
    m_bgt.Load(m_bg);
    m_bgt.SetPos(0,0);
    m_bgt.SetZoom(WIDTH,HEIGHT);
    m_bgt.SetAlpha(0);

    delete pixels;
    m_state = PAUSEING;
    m_tmr.Reset();

    //TODO:只处理了0号玩家
    player[0].ClearKey();
}

void PauseActivity::OnDraw()
{
    m_bgt_o.OnDraw();
    m_bgt.OnDraw();
}

void PauseActivity::OnNext()
{
    if(m_state == PAUSEING){
        float per = float(m_tmr.GetTimer())/100;
        if(per>=1.0){
            m_state = MENU;
            m_bgt.SetAlpha(255);
        }
        else m_bgt.SetAlpha(255*per);
    }
    if(m_state == RESUMEING){
        float per = float(m_tmr.GetTimer())/100;
        if(per>=1.0) {
            wstg -> OnResume();
            Snow::Return();
        }
        else m_bgt.SetAlpha(255-255*per);
    }

}

void PauseActivity::OnEvent(int p, Key k, bool b)
{
    if((k == T_ENTER || k == T_ESC || k == T_PAUSE)&&b){
        ResumeGame();
    }
}

void PauseActivity::ResumeGame()
{
    m_state = RESUMEING;
    m_tmr.Reset();
}

void PauseActivity::OnHide()
{

}
