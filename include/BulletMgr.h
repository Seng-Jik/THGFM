#ifndef BULLETMGR_H
#define BULLETMGR_H
#include <queue>
#include "Snow.h"
#include "StageMgr.h"
struct Bullet{
    bool live;  //生存
    bool collateEnabled;    //已启用碰撞检测
    int style;  //风格
    int cnt,minLiveTime;    //帧计数器，最短生存时间
    double x,y,angle,spd;   //x,y,飞行角度,速度
    double state[8];
    Uint8 alpha = 0;
    bool alpha_living = false;

    double self_angle = 0;    //旋转角度
    int self_w; //子弹自我缩放，只适用于四边形子弹
    int self_h;
    int self_roll_center_x,self_roll_center_y;

    Shot* link;
    int linkNum;

    enum{SHOWING,HIDING,NONE}ani;   //动画类型
    float aniState[2];
    //圆形弹幕中
    //  0号为动画百分比
};
class BulletMgr
{
    private:
        Bullet m_blts[1500];
        std::priority_queue<int> m_freeList;
        struct BulletStyle{
            SDL_Texture* tex;
            enum{CIRCLE,XRECT}shape = CIRCLE; //刚体形状
            union{
                struct{
                    double r;
                    double coll_r;
                }circle;
                struct{
                    int w,h;
                }xrect;
            };

        };
        static BulletStyle m_bulletStyle[64];
        int m_searchTop = 0;

        void enableXRectBullet(int bltNum);
        static void loadCircleBulletStyle(int i,const std::string&,double r=8,double coll_r=6);
        static void loadXRectBulletStyle(int i,const std::string&,int w,int h);
    public:
        static void Init();
        BulletMgr();
        void OnNext();
        void OnDraw();
        void Clear();
        int Alloc(double x,double y,int style);
        void Kill(int n);
        void KillBulletAndInstallEffect(int n);
        Bullet& operator [] (int n){return m_blts[n];}
        inline int GetSearchTop(){return m_searchTop;}
        ~BulletMgr();
    protected:

};

extern BulletMgr bulletMgr;

#endif // BULLETMGR_H
