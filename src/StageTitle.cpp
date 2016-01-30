#include "StageTitle.h"
#include "Tools.h"

StageTitle stageTitle;
#include "Snow/Debug.h"

void StageTitle::Set(int beg, int end, const std::string& image)
{
    m_begTime = beg;
    m_endTime = end;
    m_tex = LoadPic(image);
    r.x = r.y = 0;
    SDL_QueryTexture(m_tex,nullptr,nullptr,&m_w,&m_h);
    r.w = m_w;
    r.h = m_h;
    m_timeLen = m_endTime - m_begTime;
    m_cnt = 0;
    SDL_SetTextureAlphaMod(m_tex,0);
    //PNT(image);
}

void StageTitle::OnNext()
{
    if(m_cnt <= 120){
        SDL_SetTextureAlphaMod(m_tex,float(m_cnt)/120*255);
    }else if(m_cnt >= m_timeLen - 120){
        SDL_SetTextureAlphaMod(m_tex,255*(float(m_timeLen-m_cnt)/120));
    }
    ++m_cnt;
}

void StageTitle::OnDraw()
{
    SDL_RenderCopy(Snow::pRnd,m_tex,nullptr,&r);
}
