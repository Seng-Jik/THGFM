#include "StageMgr.h"
#include "BulletMgr.h"
#include "StdParttenAPI.h"

/* ����Boss������Ч��API */
typedef void(*SCPartten)(Boss* b,int cnt,int scnt,int& image,double& boss_x,double& boss_y,double& boss_spd,double& boss_aspd,double& boss_angle,double hp,const std::vector<int>& bullets,Snow::Bundle<256>& data);
//Boss ָ�룬Boss��֡������������֡����ʹ���е�Bossͼ��Boss����x��Boss����y��Boss�ٶȣ�Boss���ٶȣ�Boss�ƶ��Ƕȣ���������ʣ�࣬�ӵ���
extern SCPartten scPartten [];

/* ͨ��API */
#define BltAngle(b) bulletMgr[b].angle
#define BltSpd(b) bulletMgr[b].spd
#define BltState(b,i) bulletMgr[b].state[i]
#define BltSelfAngle(b) bulletMgr[b].self_angle
#define BltSelfW(b) bulletMgr[b].self_w
#define BltSelfH(b) bulletMgr[b].self_h
#define BltKill(b) bulletMgr.Kill(b)
#include "Snow/Debug.h"
#include "Boss.h"
inline int SCCreateBlt(Boss* s,double x,double y,double angle,double spd,int minLiveTime,int style)
{
    int n = bulletMgr.Alloc(x,y,style);
    if(n == -1) return -1;
    bulletMgr[n].angle = angle;
    bulletMgr[n].self_angle = angle;
    bulletMgr[n].spd = spd;
    bulletMgr[n].minLiveTime = minLiveTime;
    bulletMgr[n].link = nullptr;
    bulletMgr[n].linkNum = s ->m_bullets.size();
    s -> m_bullets.push_back(n);
    return n;
}

inline float GetRandomEnemyAngle(){
    auto pEne = stage.GetEnemy();
    if(pEne){
        return EneGetPlrAngle(0,pEne->num);
    }else if(stage.GetBoss()){
        double x,y,x2,y2;
        PlrGetPos(0,x,y);
        stage.GetBoss()->GetPos(x2,y2);
        return StdGetAngle(x,y,x2,y2);
    }
}
