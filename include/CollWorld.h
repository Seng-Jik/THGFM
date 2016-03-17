#pragma once
#include <iostream>
#include "Snow.h"
using namespace std;
class Boss;
class CollWorld{
private:
    struct CollRect{
        double x,y,w = 4,h = 4;
        bool enable = false;
    };
    struct CollCircle{
        double x,y,r = 4;
        bool enable = false;
    };
    CollCircle m_player[2];

    CollCircle* m_enemys = nullptr;
    int m_enemySize = 0;
    int m_enemySearchTop = 0;
    int m_enemySearchBotton = 0;

    CollCircle m_enemyBullets [1500];
    int m_enemyBulletSearchTop = 0;

    CollRect m_playerBullets [512];
    int m_playerBulletSearchTop = 0;

    //Enemy_PlayerBullet 方框剪裁优化
    SDL_Rect m_enemy_playerBullet_opmRect;

    //Boss CollWorld
    CollCircle m_boss;
    Boss* m_bossObject;

    struct CollXRect{
        bool enable = false;
        int x[4],y[4];
    };
    CollXRect m_enemyBulletsXRect[1500];

    inline bool coll_r2c(const CollRect& r,const CollCircle& c){
        double x1 = r.x - c.r;
        double y1 = r.y - c.r;
        double x2 = r.x + r.w + c.r;
        double y2 = r.y + r.h + c.r;
        return c.x > x1 && c.x < x2 && c.y > y1 && c.y < y2;
    }

    inline bool coll_c2c(const CollCircle& c1,const CollCircle& c2){
        double w = c1.x - c2.x;
        double h = c1.y - c2.y;
        double r = c1.r + c2.r;
        return r*r >= w*w+h*h;
    }

    CollRect m_booms [4];

public:

    inline void SetBoom(bool enable,int i,int x,int y,int w,int h){
        m_booms[i].enable = enable;
        m_booms[i].x = x;
        m_booms[i].y = y;
        m_booms[i].w = w;
        m_booms[i].h = h;
    }

    inline void SetPlayerBulletYRect(int top,int bottom){
        m_enemy_playerBullet_opmRect.y = top;
        m_enemy_playerBullet_opmRect.h = bottom - top;
    }

    inline void SetEnemyXRect(int left,int right){
        m_enemy_playerBullet_opmRect.x = left;
        m_enemy_playerBullet_opmRect.w = right - left;
    }

    inline void SetPlayer(int p,bool enable,double x = 0,double y = 0)  //设置玩家，判定点为矩形
    {
        m_player[p].enable = enable;
        m_player[p].x = x;
        m_player[p].y = y;
        m_player[p].r = 1;
    }

    inline void SetPlayerBullet(int num,bool enable,double x,double x_old,double w,double y,double h,int searchTop){
        m_playerBulletSearchTop = searchTop;

        m_playerBullets[num].enable = enable;
        m_playerBullets[num].x = x_old + w/2;
        m_playerBullets[num].y = y - h/2;
        m_playerBullets[num].w = x - x_old;
        m_playerBullets[num].h = h;
    }

    void SetEnemyBuffurSize(int n);
    inline void SetEnemy(int num,bool enable,double x,double y,double r)   //设置敌人，判定点为圆形
    {
        m_enemys[num].enable = enable;
        m_enemys[num].x = x;
        m_enemys[num].y = y;
        m_enemys[num].r = r;
    }

    inline void SetEnemyBulletCircle(int num,bool enable,double x=0,double y=0,double r=0) //设置敌人子弹，判定点为圆形
    {
        m_enemyBullets[num].enable = enable;
        m_enemyBullets[num].x = x;
        m_enemyBullets[num].y = y;
        m_enemyBullets[num].r = r;
    }

    inline void SetEnemyBulletXRect(int num,bool enable,int* x=nullptr,int* y = nullptr){   //设置敌人子弹，判定点为四边形，各点逆时针顺序排列
        m_enemyBulletsXRect[num].enable = enable;
        if(enable) for(int i = 0;i < 4;++i){
            m_enemyBulletsXRect[num].x[i] = x[i];
            m_enemyBulletsXRect[num].y[i] = y[i];
        }
    };

    inline void SetEnemySearchTop(int top,int Bottom){
        m_enemySearchTop = top;
        m_enemySearchBotton = Bottom;
    }

    inline void SetEnemyBulletSearchTop(int top)  //敌人子弹搜索顶部
    {m_enemyBulletSearchTop = top;}

    inline void SetBossObj(Boss* b){m_bossObject = b;}
    inline void SetBossEnemy(bool enable,int x,int y,int r){
        m_boss.enable = enable;
        m_boss.x = x;
        m_boss.y = y;
        m_boss.r = r;
    }

    void ClearEnemyBullet();
    void ClearEnemy();
    void ClearBoom();

    void Update_Player_Enemy();
    void Update_Player_EnemyBullet();
    void Update_Enemy_PlayerBullet();
    void Update_Boom();

    void Debug_DrawEnemy();
    void Debug_DrawEnemyBullet();
    void Debug_DrawPlayer();
    void Debug_DrawPlayerBullet();
    void Debug_DrawEnemy_XRect();

    void OnPlayerEnemy(int player,int enemy); //敌人与玩家碰撞
    void OnPlayerEnemyBullet(int player,int eb);    //玩家与敌人子弹碰撞
};

extern CollWorld collWorld;
