/* XRect 对点进行碰撞检测所需要的数学问题 */

/* 直线方程 */
struct XFLine{
    double k,b;
};

/* 给出两个点求直线方程 */
static inline void GetLineXF(XFLine& xf,double x1,double y1,double x2,double y2){
    if(x2 == x1) xf.k = 100000;
    else xf.k = float(y2-y1)/(x2-x1);
    xf.b = y1 - xf.k*x1;
}

/* 带入直线方程 */
static double GetY(const XFLine& xf,double x){
    return xf.k*x+xf.b;
}

/* 求证点在两条直线之间 */
static bool inline PointIn2XFLines(const XFLine& xf1,const XFLine& xf2,double x,double y){
    double y1 = GetY(xf1,x);
    double y2 = GetY(xf2,x);
    return (y>=y1 && y<=y2)||(y<=y1&&y>=y2);
}

/* 求证点在四条直线围成的四边形内 */
static bool inline PointIn4XFLinesRect(const XFLine* xfs,double x,double y){
    return PointIn2XFLines(xfs[0],xfs[2],x,y) && PointIn2XFLines(xfs[1],xfs[3],x,y);
}

/* 求证点在四个点围成的四边形内 */
static bool inline PointIn4PointsRect(int* x,int* y,int x0,int y0){
    XFLine xfs[4];
    for(int i = 0;i < 3;++i){
        GetLineXF(xfs[i],x[i],y[i],x[i+1],y[i+1]);
    }
    GetLineXF(xfs[3],x[3],y[3],x[0],y[0]);
    return PointIn4XFLinesRect(xfs,x0,y0);
}


#include "BulletMgr.h"
#include "CollWorld.h"
#include <stdlib.h>
#include "Snow.h"
#include "Player.h"
#include "StageMgr.h"
#include "PlayerBullet.h"
#include "PointInXRect.h"
#include "Snow/Debug.h"
using namespace Snow;

CollWorld collWorld;

inline static bool CInRect(const SDL_Rect& r,int x,int y){
    return x>=r.x && x<=r.x+r.w && y>=r.y && y<=r.y+r.h;
}

void CollWorld::SetEnemyBuffurSize(int n)
{
    delete [] m_enemys;
    m_enemys = new CollCircle [n];
    m_enemySearchBotton = 0;
    m_enemySearchTop = 0;
    m_enemySize = n;
    for(int i = 0;i < n;++i)
        m_enemys[i].enable = false;
}

void CollWorld::ClearEnemy()
{
    m_enemySearchBotton = m_enemySearchTop = 0;
    for(int i = 0;i < m_enemySize;++i)
        m_enemys[i].enable = false;
    m_boss.enable = false;
    m_boss.r = 64;
}


void CollWorld::ClearEnemyBullet()
{
    m_enemyBulletSearchTop = 0;
    memset(m_enemyBullets,0,1500*sizeof(CollCircle));
    memset(m_enemyBulletsXRect,0,1500*sizeof(CollXRect));
}

void CollWorld::Debug_DrawEnemy()
{
    for(int i = m_enemySearchBotton;i < m_enemySearchTop;++i){
        if(m_enemys[i].enable && CInRect(m_enemy_playerBullet_opmRect,m_enemys[i].x,m_enemys[i].y)){
            SDL_SetRenderDrawColor(pRnd,0,255,255,92);
            SDL_Rect r = {int(m_enemys[i].x - m_enemys[i].r),
                                int(m_enemys[i].y - m_enemys[i].r),
                                int(2*m_enemys[i].r),
                                int(2*m_enemys[i].r)
            };
            SDL_RenderFillRect(pRnd,&r);
            char buf [4];
            sprintf(buf,"%d",i);
            SDL_SetRenderDrawColor(pRnd,255,255,255,255);
            SDLTest_DrawString(pRnd,r.x,r.y,buf);
        }
    }

    //Draw Boss
    SDL_SetRenderDrawColor(pRnd,0,255,255,92);
    SDL_Rect r = {int(m_boss.x - m_boss.r),
                            int(m_boss.y - m_boss.r),
                            int(2*m_boss.r),
                            int(2*m_boss.r)
    };
    if(m_boss.enable) SDL_RenderFillRect(pRnd,&r);
}

void CollWorld::Debug_DrawEnemyBullet()
{
    for(int i = 0;i < m_enemyBulletSearchTop;++i){
        if(m_enemyBullets[i].enable){
            SDL_SetRenderDrawColor(pRnd,0,255,0,255);
            SDL_Rect r = {int(m_enemyBullets[i].x - m_enemyBullets[i].r),
                                int(m_enemyBullets[i].y - m_enemyBullets[i].r),
                                int(2*m_enemyBullets[i].r),
                                int(2*m_enemyBullets[i].r)
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
            SDL_Rect r = {int(m_player[i].x),int(m_player[i].y),int(m_player[i].r),int(m_player[i].r)};
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
                if(coll_c2c(m_player[player],m_enemys[i]))
                    OnPlayerEnemy(player,i);
        if(m_boss.enable)
            if(coll_c2c(m_player[player],m_boss))
                    OnPlayerEnemy(player,-1);
    }
}

void CollWorld::Update_Player_EnemyBullet()
{
    for(int player = 0;player < 2;++player){
        if(!m_player[player].enable) continue;
        for(int i = 0;i < m_enemyBulletSearchTop;++i){
            if(m_enemyBullets[i].enable){
                if(coll_c2c(m_player[player],m_enemyBullets[i])){
                    OnPlayerEnemyBullet(player,i);
                }
            }
            else if(m_enemyBulletsXRect[i].enable){
                if(PointIn4PointsRect(m_enemyBulletsXRect[i].x,m_enemyBulletsXRect[i].y,m_player[player].x,m_player[player].y)){
                    OnPlayerEnemyBullet(player,i);
                }
            }
        }
    }
}


void CollWorld::Update_Enemy_PlayerBullet()
{
    static int bulletNeedTest[1500];
    int bulletNTSize = -1;

    for(int enemyNum = m_enemySearchBotton;enemyNum < m_enemySearchTop;++enemyNum){
        if(InRect(m_enemy_playerBullet_opmRect,m_enemys[enemyNum].x,m_enemys[enemyNum].y) && m_enemys[enemyNum].enable){
            if(bulletNTSize == -1){
                bulletNTSize = 0;
                for(int playerBulletNum = 0;playerBulletNum < m_playerBulletSearchTop;++playerBulletNum){
                    if(m_playerBullets[playerBulletNum].enable &&
                        InRect(m_enemy_playerBullet_opmRect,m_playerBullets[playerBulletNum].x,m_playerBullets[playerBulletNum].y)
                        ){
                            bulletNeedTest[bulletNTSize++] = playerBulletNum;
                            if(coll_r2c(m_playerBullets[playerBulletNum],m_enemys[enemyNum])){
                                stage.KillEnemy(enemyNum,playerBulletMgr.GetPower(playerBulletNum));
                                playerBulletMgr.Kill(playerBulletNum);
                                //PNT("CollWorld::Enemy_PlayerBullet:"<<enemyNum<<"  "<<playerBulletNum);
                            }
                        }
                }
            }else{
                for(int playerBulletNum = 0;playerBulletNum < bulletNTSize;++playerBulletNum){
                    if(m_playerBullets[bulletNeedTest[playerBulletNum]].enable && m_enemys[enemyNum].enable)
                        if(coll_r2c(m_playerBullets[bulletNeedTest[playerBulletNum]],m_enemys[enemyNum])){
                            stage.KillEnemy(enemyNum,playerBulletMgr.GetPower(bulletNeedTest[playerBulletNum]));
                            playerBulletMgr.Kill(bulletNeedTest[playerBulletNum]);
                        }
                }
            }
        }
    }

    //Boss
    if(m_boss.enable) for(int playerBulletNum = 0;playerBulletNum < m_playerBulletSearchTop;++playerBulletNum){
        if(coll_r2c(m_playerBullets[playerBulletNum],m_boss)){
            m_bossObject -> KillHP(playerBulletMgr.GetPower(playerBulletNum));
            playerBulletMgr.Kill(playerBulletNum);
        }
    }
    //PNT("RECT"<<m_enemy_playerBullet_opmRect.x<<" "<<m_enemy_playerBullet_opmRect.y<<" "<<m_enemy_playerBullet_opmRect.w<<" "<<m_enemy_playerBullet_opmRect.h);
}

void CollWorld::Debug_DrawPlayerBullet()
{
    SDL_SetRenderDrawColor(pRnd,255,0,0,128);
    for(int i = 0;i < 512;++i){
        CollRect& c = m_playerBullets[i];
        if(c.enable && CInRect(m_enemy_playerBullet_opmRect,c.x,c.y)){
            SDL_Rect r = {
                int(c.x),
                int(c.y),
                int(c.w),
                int(c.h)
            };
            SDL_RenderFillRect(pRnd,&r);
        }
    }
    SDL_SetRenderDrawColor(pRnd,255,0,0,96);
    SDL_RenderFillRect(pRnd,&m_enemy_playerBullet_opmRect);
}

void CollWorld::Debug_DrawEnemy_XRect()
{
    SDL_SetRenderDrawColor(pRnd,255,0,0,255);
    for(int j = 0;j < m_enemyBulletSearchTop;++j){
        if(m_enemyBulletsXRect[j].enable){
            for(int i = 0;i < 3;++i){
                SDL_RenderDrawLine(pRnd,m_enemyBulletsXRect[j].x[i],m_enemyBulletsXRect[j].y[i],m_enemyBulletsXRect[j].x[i+1],m_enemyBulletsXRect[j].y[i+1]);
            }
            SDL_RenderDrawLine(pRnd,m_enemyBulletsXRect[j].x[0],m_enemyBulletsXRect[j].y[0],m_enemyBulletsXRect[j].x[3],m_enemyBulletsXRect[j].y[3]);

            /* 外加方程线 */
            XFLine xfs[4];
            for(int i = 0;i < 3;++i){
                GetLineXF(xfs[i],m_enemyBulletsXRect[j].x[i],m_enemyBulletsXRect[j].y[i],m_enemyBulletsXRect[j].x[i+1],m_enemyBulletsXRect[j].y[i+1]);
            }
            GetLineXF(xfs[3],m_enemyBulletsXRect[j].x[3],m_enemyBulletsXRect[j].y[3],m_enemyBulletsXRect[j].x[0],m_enemyBulletsXRect[j].y[0]);
            for(int i = 0;i < 4;++i)
                SDL_RenderDrawLine(pRnd,0,GetY(xfs[i],0),1280,GetY(xfs[i],1280));
        }
    }
}

void CollWorld::Update_Boom()
{
    for(int boom = 0;boom < 4;++boom){
        if(!m_booms[boom].enable) continue;

        //炸弹消除敌人
        for(int i = m_enemySearchBotton;i < m_enemySearchTop;++i){
            if(m_booms[boom].enable && coll_r2c(m_booms[boom],m_enemys[i])){
                stage.KillEnemy(i,5);
            }
        }
        //炸弹攻击Boss
        if(m_boss.enable && coll_r2c(m_booms[boom],m_boss)){
            m_bossObject -> KillHP(1);
        }
        //炸弹抵消子弹
        for(int i = 0;i < m_enemyBulletSearchTop;++i){
            //圆形
            if(m_enemyBullets[i].enable && coll_r2c(m_booms[boom],m_enemyBullets[i])){
                m_enemyBullets[i].enable = false;
                bulletMgr.KillBulletAndInstallEffect(i);
            }
            //方形
            if(m_enemyBulletsXRect[i].enable){
                bool AllIn = true;
                for(int j = 0;j < 4;++j){
                    SDL_Rect r = {(int)m_booms[boom].x,(int)m_booms[boom].y,(int)m_booms[boom].w,(int)m_booms[boom].h};
                    if(!InRect(r,m_enemyBulletsXRect[i].x[j],m_enemyBulletsXRect[i].y[j])){
                        AllIn = false;break;
                    }
                }
                if(AllIn){
                    m_enemyBullets[i].enable = false;
                    bulletMgr.KillBulletAndInstallEffect(i);
                }
            }
        }
    }
}

void CollWorld::ClearBoom()
{
    for(int i = 0;i < 4;++i){
        m_booms[i].enable = false;
    }
}




#include "EffectMgr.h"
#include "GameUI.h"
void CollWorld::OnPlayerEnemy(int player, int enemy)
{
    ::player[player].Kill();
    //gameUI.OnPlayerBeKilled();
    //SDL_assert(false);
}

void CollWorld::OnPlayerEnemyBullet(int player, int eb)
{
    ::player[player].Kill();
    //gameUI.OnPlayerBeKilled();
    //SDL_assert(false);
}
