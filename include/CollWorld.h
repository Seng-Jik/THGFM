#pragma once
#include <iostream>
using namespace std;
class CollWorld{
private:
    struct CollRect{
        int x,y,w = 4,h = 4;
        bool enable = false;
    };
    struct CollCircle{
        int x,y,r;
        bool enable = false;
    };
    CollRect m_player[2];

    CollCircle* m_enemys = nullptr;
    int m_enemySearchTop = 0;
    int m_enemySearchBotton = 0;

    CollCircle m_enemyBullets [1500];
    int m_enemyBulletSearchTop = 0;

    inline bool coll_r2c(const CollRect& r,const CollCircle& c){
        int x1 = r.x - c.r;
        int y1 = r.y - c.r;
        int x2 = r.x + r.w + c.r;
        int y2 = r.y + r.h + c.r;
        return c.x > x1 && c.x < x2 && c.y > y1 && c.y < y2;
    }

public:
    inline void SetPlayer(int p,bool enable,int x = 0,int y = 0)  //设置玩家，判定点为矩形
    {
        m_player[p].enable = enable;
        m_player[p].x = x;
        m_player[p].y = y;
    }

    void SetEnemyBuffurSize(int n);
    inline void SetEnemy(int num,bool enable,int x = 0,int y = 0,int r = 0)   //设置敌人，判定点为圆形
    {
        m_enemys[num].enable = enable;
        m_enemys[num].x = x;
        m_enemys[num].y = y;
        m_enemys[num].r = r;
    }

    inline void SetEnemySearchBottom(int bottom)    //设置敌人搜索底部
    {m_enemySearchBotton = bottom;}

    inline void SetEnemySearchTop(int top)    //设置敌人搜索顶部
    {m_enemySearchTop = top;}

    inline void SetEnemyBullet(int num,bool enable,int x=0,int y=0,int r=0) //设置敌人子弹，判定点为圆形
    {
        m_enemyBullets[num].enable = enable;
        m_enemyBullets[num].x = x;
        m_enemyBullets[num].y = y;
        m_enemyBullets[num].r = r;
    }

    inline void SetEnemyBulletSearchTop(int top)  //敌人子弹搜索顶部
    {m_enemyBulletSearchTop = top;}

    void ClearEnemyBullet();
    void ClearEnemy();

    void Update_Player_Enemy();
    void Update_Player_EnemyBullet();

    void Debug_DrawEnemy();
    void Debug_DrawEnemyBullet();
    void Debug_DrawPlayer();

    void OnPlayerEnemy(int player,int enemy); //敌人与玩家碰撞
    void OnPlayerEnemyBullet(int player,int eb);    //玩家与敌人子弹碰撞
};

extern CollWorld collWorld;
