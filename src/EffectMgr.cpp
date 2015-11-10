#include "EffectMgr.h"
#include "Tools.h"
using namespace std;

EffectMgr::EffectMgr()
{
    //ctor
}

EffectMgr::~EffectMgr()
{
    //dtor
}
void EffectMgr::Init()
{
    m_effStyles[0].frameWait =5;
    m_effStyles[0].tex = new SDL_Texture* [4];
    m_effStyles[0].tex[0]=LoadPic("Effect/boom1.png");
    m_effStyles[0].tex[1]=LoadPic("Effect/boom2.png");
    m_effStyles[0].tex[2]=LoadPic("Effect/boom3.png");
    m_effStyles[0].tex[3]=LoadPic("Effect/boom4.png");
    m_effStyles.texCount = 4;
}

void EffectMgr::Install(int num, int x, int y)
{
    m_effs.push_back( {num,0,x,y});
}

void EffectMgr::OnDraw()
{
    FOR_EACH(p,m_effs.begin(),m_effs.end()){
        int w,h;
        SDL_QueryTexture(m_effStyles[p->style].tex[p->cnt%m_effStyles[p->style].frameWait],nullptr,nullptr,&w,&h);
        SDL_Rect dst = {x-w/2,y-h/w,w,h};
        SDL_RenderCopy(pRnd,m_effStyles[p->style].tex[p->cnt%m_effStyles[p->style].frameWait,nullptr,&dst);
    }
}

void EffectMgr::OnNext()
{
    FOR_EACH(p,m_effs.begin(),m_effs.end()){
        p->cnt++;
    }
}

void EffectMgr::Reset()
{
    m_effs.clear();
}
