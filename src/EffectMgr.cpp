#include "EffectMgr.h"
#include "Tools.h"
#include "Snow.h"
using namespace Snow;
using namespace std;

EffectMgr effMgr;

EffectMgr::EffectStyle EffectMgr::m_effStyles[1];

EffectMgr::EffectMgr()
{
    Reset();
}

EffectMgr::~EffectMgr()
{
    //dtor
}
void EffectMgr::Init()
{
    m_effStyles[0].frameWait =4;
    m_effStyles[0].tex = new SDL_Texture* [4];
    m_effStyles[0].tex[0]=LoadPic("Effect/boom1.png");
    m_effStyles[0].tex[1]=LoadPic("Effect/boom2.png");
    m_effStyles[0].tex[2]=LoadPic("Effect/boom3.png");
    m_effStyles[0].tex[3]=LoadPic("Effect/boom4.png");
    FOR_EACH(i,0,4){
        SDL_SetTextureAlphaMod(m_effStyles[0].tex[i],160);
    }
    m_effStyles[0].texCount = 4;
}

void EffectMgr::Install(int style, int x, int y)
{
    for(int i = 0;i <32;++i){
        if(m_effs[i].cnt == -1){
            m_effs[i].cnt = 0;
            m_effs[i].x = x;
            m_effs[i].y = y;
            m_effs[i].style = style;
            break;
        }
    }
}

void EffectMgr::OnDraw()
{
    for(int i = 0;i < 32;++i)
        if(m_effs[i].cnt != -1){
            int w,h;
            int texNum = m_effs[i].cnt / m_effStyles[m_effs[i].style].frameWait;
            if(texNum >= m_effStyles[m_effs[i].style].texCount) texNum = m_effStyles[m_effs[i].style].texCount -1;
            SDL_QueryTexture(m_effStyles[m_effs[i].style].tex[texNum],nullptr,nullptr,&w,&h);
            SDL_Rect dst = {
                m_effs[i].x - w/2,
                m_effs[i].y - h/2,
                w,h
            };
            SDL_RenderCopy(pRnd,m_effStyles[m_effs[i].style].tex[texNum],nullptr,&dst);
    }
}

void EffectMgr::OnNext()
{
    for(int i = 0;i < 32;++i)
        if(m_effs[i].cnt != -1){
            ++m_effs[i].cnt;
            if(m_effs[i].cnt>=m_effStyles[m_effs[i].style].frameWait*m_effStyles[m_effs[i].style].texCount)
                m_effs[i].cnt = -1;
    }
}
#include <cstring>
void EffectMgr::Reset()
{
    for(int i = 0;i < 32;++i)
        m_effs[i].cnt = -1;
}
