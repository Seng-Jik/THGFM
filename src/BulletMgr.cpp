#include "BulletMgr.h"
#include "Tools.h"
#include <cmath>
#include "CollWorld.h"
using namespace Snow;

BulletMgr bulletMgr;

BulletMgr::BulletStyle BulletMgr::m_bulletStyle[10];
void BulletMgr::Init()
{
    m_bulletStyle[0].tex = LoadPic("Bullet/s1.png");
    SDL_SetTextureBlendMode(m_bulletStyle[0].tex,SDL_BLENDMODE_BLEND);
    m_bulletStyle[0].r = 8;
    m_bulletStyle[0].coll_r = 5;
}


BulletMgr::BulletMgr()
{
    Clear();
}

BulletMgr::~BulletMgr()
{
    //dtor
};

void BulletMgr::OnNext()
{
    for(int i = 0;i < m_searchTop;++i){
        if(m_blts[i].live){
            if(m_blts[i].alpha < 250 && m_blts[i].alpha_living){
                m_blts[i].alpha+=25;
                if(m_blts[i].alpha == 255) m_blts[i].alpha_living = false;
            }
            m_blts[i].x -= m_blts[i].spd * cos(m_blts[i].angle);
            m_blts[i].y -= m_blts[i].spd * sin(m_blts[i].angle);
            ++m_blts[i].cnt;
            collWorld.SetEnemyBullet(i,true,m_blts[i].x,m_blts[i].y,m_bulletStyle[m_blts[i].style].coll_r);
            if(m_blts[i].minLiveTime < m_blts[i].cnt && (
               m_blts[i].x > WIDTH+ m_bulletStyle[m_blts[i].style].r||
               m_blts[i].x < -m_bulletStyle[m_blts[i].style].r ||
               m_blts[i].y > HEIGHT+ m_bulletStyle[m_blts[i].style].r||
               m_blts[i].y < -m_bulletStyle[m_blts[i].style].r)
               )
                Kill(i);
        }
    }
    collWorld.Update_Player_EnemyBullet();
}

void BulletMgr::OnDraw()
{
    for(int i = 0;i < m_searchTop;++i){
        if(m_blts[i].live){
            SDL_Rect r = {int(m_blts[i].x - m_bulletStyle[m_blts[i].style].r),
                                int(m_blts[i].y - m_bulletStyle[m_blts[i].style].r),
                                int(2*m_bulletStyle[m_blts[i].style].r),
                                int(2*m_bulletStyle[m_blts[i].style].r)
            };
            SDL_SetTextureAlphaMod(m_bulletStyle[m_blts[i].style].tex,m_blts[i].alpha);
            SDL_Point poi = {m_bulletStyle[m_blts[i].style].r,m_bulletStyle[m_blts[i].style].r};
            SDL_RenderCopyEx(pRnd,m_bulletStyle[m_blts[i].style].tex,nullptr,&r,m_blts[i].render_angle,&poi,SDL_FLIP_NONE);
        }
    }
}

void BulletMgr::Clear()
{
    while(!m_freeList.empty()) m_freeList.pop();
    for(int i = 0;i < 700;++i){
        m_freeList.push(i);
        m_blts[i].live = false;
    }
    m_searchTop = 0;
    collWorld.ClearEnemyBullet();
}
int BulletMgr::Alloc()
{
    if(m_freeList.empty()) return -1;
    int n = 699 - m_freeList.top();
    m_freeList.pop();
    if(n >= m_searchTop) m_searchTop = n+1;
    collWorld.SetEnemyBulletSearchTop(m_searchTop);

    m_blts[n].live = true;
    m_blts[n].cnt = 0;
    m_blts[n].alpha = 0;
    m_blts[n].alpha_living = true;

    return n;
}

void BulletMgr::Kill(int n)
{
    m_blts[n].live = false;
    m_freeList.push(699-n);
    while(!m_blts[m_searchTop - 1].live)
        --m_searchTop;
    collWorld.SetEnemyBullet(n,false);
    collWorld.SetEnemyBulletSearchTop(m_searchTop);
}
