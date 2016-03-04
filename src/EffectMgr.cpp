#include "EffectMgr.h"
#include "Tools.h"
#include "Snow.h"
#include "Snow/Debug.h"
using namespace Snow;
using namespace std;

EffectMgr effMgr;

EffectMgr::EffectStyle EffectMgr::m_effStyles[32];

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
    Snow::CSVReader csv;
    csv.LoadCSV("Effect/styles.csv");
    int num = 0;
    csv.NextLine();
    do{
        csv.PopInt(m_effStyles[num].texCount);
        csv.PopInt(m_effStyles[num].frameWait);
        m_effStyles[num].tex = new SDL_Texture* [m_effStyles[num].texCount];
        for(int i = 0;i < m_effStyles[num].texCount;++i){
            m_effStyles[num].tex[i] = LoadPic("Effect/eff"+std::to_string(num)+"/"+std::to_string(i+1)+".png");
        }
        ++num;
    }while(csv.NextLine());
}

void EffectMgr::InstallFrameAnimation(int style, int x, int y)
{
    PNT("SSSSSS");
    for(int i = 0;i <1024;++i){
        if(m_effs[i].style == -1){
            m_effs[i].cnt = 0;
            m_effs[i].x = x;
            m_effs[i].y = y;
            m_effs[i].style = style;

            if(i >= m_searchTop-1){
                m_searchTop = i + 1;
            }

            break;
        }
    }
}

void EffectMgr::InstallZoomOutAnimation(SDL_Texture* image,const SDL_Rect& r,bool alphaDown,const SDL_Point& poi,const double angle)
{
    //寻找一个空着的特效位置
    int num = 0;
    for(num = 0;num<m_zoomOutEffSearchTop;++num){
        if(!m_zoomOutEffs[num].tex){
            goto FINDED;
        }
    }
    if(m_zoomOutEffSearchTop>=1024) return;
    num = m_zoomOutEffSearchTop;
    ++m_zoomOutEffSearchTop;
    //寻找结束
    FINDED:
    m_zoomOutEffs[num].tex = image;
    m_zoomOutEffs[num].rect = r;
    m_zoomOutEffs[num].alphaDown = alphaDown;
    m_zoomOutEffs[num].alpha = 255;
    m_zoomOutEffs[num].centerPoi = poi;
    m_zoomOutEffs[num].angle = angle;
}


void EffectMgr::OnDraw()
{
    for(int i = 0;i < m_searchTop;++i)
        if(m_effs[i].style != -1){
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
    //PNT(m_searchTop);
    for(int i = 0;i < m_zoomOutEffSearchTop;++i)
        if(m_zoomOutEffs[i].tex){
            SDL_SetTextureAlphaMod(m_zoomOutEffs[i].tex,m_zoomOutEffs[i].alpha);
            SDL_RenderCopyEx(Snow::pRnd,m_zoomOutEffs[i].tex,nullptr,&m_zoomOutEffs[i].rect,m_zoomOutEffs[i].angle,&m_zoomOutEffs[i].centerPoi,SDL_FLIP_NONE);
    }
}

void EffectMgr::OnNext()
{
    for(int i = 0;i < m_searchTop;++i)
        if(m_effs[i].style != -1){
            ++m_effs[i].cnt;
            if(m_effs[i].cnt>=m_effStyles[m_effs[i].style].frameWait*m_effStyles[m_effs[i].style].texCount){
                Kill(i);
            }
    }

    for(int i = 0;i < m_zoomOutEffSearchTop;++i)
        if(m_zoomOutEffs[i].tex){
            if(m_zoomOutEffs[i].alphaDown){
                if(m_zoomOutEffs[i].alpha>= 20)
                    m_zoomOutEffs[i].alpha -= 20;
                else{
                    m_zoomOutEffs[i].tex = nullptr;
                }
            }
            m_zoomOutEffs[i].rect.x+= 1;
            m_zoomOutEffs[i].rect.y+= 1;
            m_zoomOutEffs[i].rect.w-= 2;
            m_zoomOutEffs[i].rect.h-= 2;
            if(m_zoomOutEffs[i].rect.w<=0 || m_zoomOutEffs[i].rect.h<=0)
                m_zoomOutEffs[i].tex = nullptr;
    }
    while(!m_zoomOutEffs[m_zoomOutEffSearchTop-1].tex && m_zoomOutEffSearchTop>=1){
        m_zoomOutEffSearchTop--;
    }
}

void EffectMgr::Reset()
{
    for(int i = 0;i < 1024;++i){
        m_zoomOutEffs[i].tex = nullptr;
        m_effs[i].style = -1;
    }
    m_searchTop = 0;
    m_zoomOutEffSearchTop = 0;
}

void EffectMgr::Kill(int n)
{
    m_effs[n].style = -1;
    while(m_effs[m_searchTop-1].style == -1)
        --m_searchTop;
}
