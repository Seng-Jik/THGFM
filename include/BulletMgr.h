#ifndef BULLETMGR_H
#define BULLETMGR_H
#include <queue>
#include "Snow.h"
#include "StageMgr.h"
struct Bullet{
    bool live;  //����
    bool collateEnabled;    //��������ײ���
    int style;  //���
    int cnt,minLiveTime;    //֡���������������ʱ��
    double x,y,angle,spd;   //x,y,���нǶ�,�ٶ�
    double state[8];
    Uint8 alpha = 0;
    bool alpha_living = false;

    double self_angle = 0;    //��ת�Ƕ�
    int self_w; //�ӵ��������ţ�ֻ�������ı����ӵ�
    int self_h;
    int self_roll_center_x,self_roll_center_y;

    Shot* link;
    int linkNum;

    enum{SHOWING,HIDING,NONE}ani;   //��������
    float aniState[2];
    //Բ�ε�Ļ��
    //  0��Ϊ�����ٷֱ�
};
class BulletMgr
{
    private:
        Bullet m_blts[1500];
        std::priority_queue<int> m_freeList;
        struct BulletStyle{
            SDL_Texture* tex;
            enum{CIRCLE,XRECT}shape = CIRCLE; //������״
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
