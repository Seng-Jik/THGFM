#include "SCClock.h"
#include "Tools.h"
#include "SeMgr.h"
ScClock scClock;

void ScClock::Init()
{
    for(int i = 0; i < 11; ++i){
        m_tex[0][i] = LoadPic("GameUI/Number/black/" + std::to_string(i) + ".PNG");
        m_tex[1][i] = LoadPic("GameUI/Number/red/" + std::to_string(i) + ".PNG");
    }
    reset();
    setAlpha(0);
}

void ScClock::setAlpha(Uint8 alpha)
{
    for(int i = 0;i < 11;++i){
        SDL_SetTextureAlphaMod(m_tex[0][i],alpha);
        SDL_SetTextureAlphaMod(m_tex[1][i],alpha);
    }
}

void ScClock::OnNext()
{
    if(!m_visible) return;
    if(m_anime == SHOWING_S1){
        float per = m_animeTmr.GetTimer() / 30.0f;
        setAlpha(per*192);
        if(per >= 1) {
            m_animeTmr.Reset();
            m_anime = SHOWING_S2;
        }
    }else if(m_anime == SHOWING_S2){
        float per = ACGCross::ArcFunc(m_animeTmr.GetTimer() / 30.0f);
        if(per == -1) per = 1;
        for(int i = 0;i < 5;++i){
            m_rect[i].y = 400 + per*150;
        }
        if(per >= 1) m_anime = NONE;
    }else if(m_anime == HIDING){
        float per = m_animeTmr.GetTimer() / 30.0f;
        setAlpha(192-per*192);
        if(per >= 1){
            m_anime = NONE;
            m_visible = false;
        }
    }
}

void ScClock::OnDraw()
{
    if(!m_visible) return;
    int c;
    if(m_sec_0 != 0) c = 0;
    else c = 1;
    if(m_sec_0 > 9 || m_sec_0 < 0) return;
    SDL_RenderCopy(Snow::pRnd, m_tex[c][m_sec_0], nullptr, &(m_rect[0]));
    if(m_sec_1 > 9 || m_sec_1 < 0) return;
    SDL_RenderCopy(Snow::pRnd, m_tex[c][m_sec_1], nullptr, &(m_rect[1]));
    SDL_RenderCopy(Snow::pRnd, m_tex[c][10], nullptr, &(m_rect[2]));
    if(m_msec_0 > 9 || m_msec_0 < 0) return;
    SDL_RenderCopy(Snow::pRnd, m_tex[c][m_msec_0], nullptr, &(m_rect[3]));
    if(m_msec_1 > 9 || m_msec_1 < 0) return;
    SDL_RenderCopy(Snow::pRnd, m_tex[c][m_msec_1], nullptr, &(m_rect[4]));
}

void ScClock::SetTime(int i_frame)
{
    m_sec_0 = i_frame / 600;
    m_sec_1 = (i_frame / 60) % 10;
    if(m_visible){
        if(!(i_frame%60)&&i_frame <=600 && i_frame>0){
            if(i_frame > 180)
                se.Play(TIMEOUT);
            else
                se.Play(TIMEOUT2);
        }
    }
    m_msec_0 = float(int(i_frame) % 60) *1000 / 60 /100;
    m_msec_1 = int(float(int(i_frame) % 60) *1000 / 60 / 10) % 10;
}

void ScClock::Show()
{
    reset();
    m_visible = true;
    m_anime = SHOWING_S1;
    m_animeTmr.Reset();
}

void ScClock::Hide()
{
    m_anime = HIDING;
    m_animeTmr.Reset();
}

void ScClock::reset()
{
    for(int i = 0; i < 5; ++i)
    {
        m_rect[i].w = 28;
        m_rect[i].h = 38;
        m_rect[i].x = (WIDTH - 28 * 5) / 2 + 28 * (i - 2) + 27*2;
        m_rect[i].y = 400;
    }
    m_rect[2].w = 13;
    m_rect[2].x = WIDTH/2 - 7;
}
