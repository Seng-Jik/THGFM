#pragma once
#include <iostream>
#include "Snow.h"
using namespace std;
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

public:

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

    inline void SetEnemyBullet(int num,bool enable,double x=0,double y=0,double r=0) //设置敌人子弹，判定点为圆形
    {
        m_enemyBullets[num].enable = enable;
        m_enemyBullets[num].x = x;
        m_enemyBullets[num].y = y;
        m_enemyBullets[num].r = r;
    }

    inline void SetEnemySearchTop(int top,int Bottom){
        m_enemySearchTop = top;
        m_enemySearchBotton = Bottom;
    }

    inline void SetEnemyBulletSearchTop(int top)  //敌人子弹搜索顶部
    {m_enemyBulletSearchTop = top;}

    void ClearEnemyBullet();
    void ClearEnemy();

    void Update_Player_Enemy();
    void Update_Player_EnemyBullet();
    void Update_Enemy_PlayerBullet();

    void Debug_DrawEnemy();
    void Debug_DrawEnemyBullet();
    void Debug_DrawPlayer();
    void Debug_DrawPlayerBullet();

    void OnPlayerEnemy(int player,int enemy); //敌人与玩家碰撞
    void OnPlayerEnemyBullet(int player,int eb);    //玩家与敌人子弹碰撞
};

extern CollWorld collWorld;
