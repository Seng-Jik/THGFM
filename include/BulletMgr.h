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
    float x,y,angle,spd;   //x,y,���нǶ�,�ٶ�
    float state[8];
    Uint8 alpha = 0;
    bool alpha_living = false;

    float self_angle = 0;    //��ת�Ƕ�
    float self_w; //�ӵ��������ţ�ֻ�������ı����ӵ�
    float self_h;
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
            enum{CIRCLE,XRECT1,XRECT2}shape = CIRCLE; //������״
            //XRect1  -- �̴�����
            //XRect2 -- ���ӵ���ֱ�ķ�����
            union{
                struct{
                    float r;
                    float coll_r;
                }circle;
                struct{
                    int w,h;
                }xrect;
            };

        };
        static BulletStyle m_bulletStyle[64];
        int m_searchTop = 0;

        void enableXRectBullet(int bltNum);
        static void loadCircleBulletStyle(int i,const std::string&,float r=8,float coll_r=6);
        static void loadXRectBulletStyle(int shape,int i,const std::string&,int w,int h);
    public:
        static void Init();
        BulletMgr();
        void OnNext();
        void OnDraw();
        void Clear();
        int Alloc(float x,float y,int style);
        void Kill(int n);
        void KillBulletAndInstallEffect(int n);
        Bullet& operator [] (int n){return m_blts[n];}
        inline int GetSearchTop(){return m_searchTop;}
        ~BulletMgr();
    protected:

};

extern BulletMgr bulletMgr;

#endif // BULLETMGR_H
