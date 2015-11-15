#include "CollWorld.h"
#include <mem.h>
#include "Snow.h"
#include "Player.h"
using namespace Snow;

CollWorld collWorld;

void CollWorld::SetEnemyBuffurSize(int n)
{
    delete [] m_enemys;
    m_enemys = new CollCircle [n];
    m_enemySearchBotton = 0;
    m_enemySearchTop = 0;
}

void CollWorld::ClearEnemyBullet()
{
    m_enemyBulletSearchTop = 0;
    memset(m_enemyBullets,0,1500*sizeof(CollCircle));
}

void CollWorld::Debug_DrawEnemy()
{
    for(int i = m_enemySearchBotton;i < m_enemySearchTop;++i){
        if(m_enemys[i].enable){
            SDL_SetRenderDrawColor(pRnd,0,255,255,92);
            SDL_Rect r = {m_enemys[i].x - m_enemys[i].r,
                                m_enemys[i].y - m_enemys[i].r,
                                2*m_enemys[i].r,
                                2*m_enemys[i].r
            };
            SDL_RenderFillRect(pRnd,&r);
            char buf [4];
            sprintf(buf,"%d",i);
            SDL_SetRenderDrawColor(pRnd,255,255,255,255);
            SDLTest_DrawString(pRnd,r.x,r.y,buf);
        }
    }
}

void CollWorld::Debug_DrawEnemyBullet()
{
    for(int i = 0;i < m_enemyBulletSearchTop;++i){
        if(m_enemyBullets[i].enable){
            SDL_SetRenderDrawColor(pRnd,0,255,0,255);
            SDL_Rect r = {m_enemyBullets[i].x - m_enemyBullets[i].r,
                                m_enemyBullets[i].y - m_enemyBullets[i].r,
                                2*m_enemyBullets[i].r,
                                2*m_enemyBullets[i].r
            };
            SDL_RenderFillRect(pRnd,&r);
            char buf [4];
            sprintf(buf,"%d",i);
            SDL_SetRenderDrawColor(pRnd,255,255,255,255);
            SDLTest_DrawString(pRnd,r.x,r.y,buf);
        }
    }
}

void CollWorld::Debug_DrawPlayer()
{
    for(int i = 0;i<2;++i){
        if(m_player[i].enable){
            SDL_SetRenderDrawColor(pRnd,0,255,0,92);
            SDL_Rect r = {m_player[i].x,m_player[i].y,m_player[i].w,m_player[i].h};
            SDL_RenderFillRect(pRnd,&r);
        }
    }
}



void CollWorld::Update_Player_Enemy()
{
    for(int player = 0;player < 2;++player){
        if(!m_player[player].enable) continue;
        for(int i = m_enemySearchBotton;i < m_enemySearchTop;++i)
            if(m_enemys[i].enable)
                if(coll_r2c(m_player[player],m_enemys[i]))
                    OnPlayerEnemy(player,i);
    }
}

void CollWorld::Update_Player_EnemyBullet()
{
    for(int player = 0;player < 2;++player){
        if(!m_player[player].enable) continue;
        for(int i = 0;i < m_enemyBulletSearchTop;++i)
            if(m_enemyBullets[i].enable)
                if(coll_r2c(m_player[player],m_enemyBullets[i])){
                    OnPlayerEnemyBullet(player,i);
                }
    }
}

#include "EffectMgr.h"
void CollWorld::OnPlayerEnemy(int player, int enemy)
{
    effMgr.Install(0,::player[player].GetX(),::player[player].GetY());
    ::player[player].Birth();
    //SDL_assert(false);
}

void CollWorld::OnPlayerEnemyBullet(int player, int eb)
{
    effMgr.Install(0,::player[player].GetX(),::player[player].GetY());
    ::player[player].Birth();
    //SDL_assert(false);
}
