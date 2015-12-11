#ifndef BULLETMGR_H
#define BULLETMGR_H
#include <queue>
#include "Snow.h"
#include "StageMgr.h"
struct Bullet{
    bool live;  //生存
    int style;  //风格
    int cnt,minLiveTime;    //帧计数器，最短生存时间
    double x,y,angle,spd;   //x,y,角度,速度
    double state[8];
    Uint8 alpha = 0;
    bool alpha_living = false;
    double render_angle = 0;

    Shot* link;
    int linkNum;
};
class BulletMgr
{
    private:
        Bullet m_blts[700];
        std::priority_queue<int> m_freeList;
        struct BulletStyle{
            SDL_Texture* tex;
            double r = 8;
            double coll_r = 6;
        };
        static BulletStyle m_bulletStyle[10];
        int m_searchTop = 0;
    public:
        static void Init();
        BulletMgr();
        void OnNext();
        void OnDraw();
        void Clear();
        int Alloc();
        void Kill(int n);
        Bullet& operator [] (int n){return m_blts[n];}
        inline int GetSearchTop(){return m_searchTop;}
        ~BulletMgr();
    protected:

};

extern BulletMgr bulletMgr;

#endif // BULLETMGR_H
