#include "SCClock.h"
#include "Tools.h"

ScClock scClock;

void ScClock::Init()
{
    for(int i = 0; i < 11; ++i)
        m_tex[i] = LoadPic("GameUI/Number/black/" + std::to_string(i) + ".PNG");
    Reset();
}

void ScClock::OnNext()
{
}

void ScClock::OnDraw()
{
    SDL_RenderCopy(Snow::pRnd, m_tex[m_sec_0], nullptr, &(m_rect[0]));
    SDL_RenderCopy(Snow::pRnd, m_tex[m_sec_1], nullptr, &(m_rect[1]));
    SDL_RenderCopy(Snow::pRnd, m_tex[10], nullptr, &(m_rect[2]));
    SDL_RenderCopy(Snow::pRnd, m_tex[m_msec_0], nullptr, &(m_rect[3]));
    SDL_RenderCopy(Snow::pRnd, m_tex[m_msec_1], nullptr, &(m_rect[4]));
}

void ScClock::SetTime(int i_frame)
{
    m_sec_0 = i_frame / 600;
    m_sec_1 = (i_frame / 60) % 10;
    m_msec_0 = float(int(i_frame) % 60) *1000 / 60 /100;
    m_msec_1 = int(float(int(i_frame) % 60) *1000 / 60 / 10) % 10;
    PNT("SCCLOCK:"<<i_frame<<" "<<m_sec_0<<m_sec_1<<":"<<m_msec_0<<m_sec_1);
}

void ScClock::Show()
{
    Reset();
    m_show = true;
    m_move = true;
    m_animetime = 60;
}

void ScClock::Hide()
{
    m_show = false;
    m_move = false;
    m_animetime += 30;
}

void ScClock::SetAlpha(float i_a)
{
    int fuck = (int)i_a;
    for(int i = 0; i < 11; ++i)
        SDL_SetTextureAlphaMod(m_tex[i], fuck);
}

void ScClock::Reset()
{
    for(int i = 0; i < 5; ++i)
    {
        m_rect[i].w = 28;
        m_rect[i].h = 38;
        m_rect[i].x = (WIDTH - 28 * 5) / 2 + 28 * (i - 2);
        m_rect[i].y = (HEIGHT - 38) / 2;
    }
    m_init = true;
}
