#include "StageMgr.h"
#include "BulletMgr.h"

/* 仅对普通敌人有效的API */
typedef void(*ShotPartten)(Shot* s,int);
//射击指针，发射者的编号
extern ShotPartten shotPartten [];

#define ShtEnd() s -> live = Shot::STOPLIVE
#define ShtArgs s -> parttenArgs
#define ShtChangePartten(parttenNum) s->partten=parttenNum;
#define ShtForBullet(i) for(int i:s->bullets) if(i!=-1)

#define ShtCnt() s -> cnt
#define ShtEnd() s -> live = Shot::STOPLIVE

static inline int ShtCreateBlt(Shot* s,double x,double y,double angle,double spd,int minLiveTime,int style)
{
    if(s->live == Shot::LIVE){
        int n = bulletMgr.Alloc(x,y,style);
        if(n == -1) return -1;
        bulletMgr[n].angle = angle;
        bulletMgr[n].self_angle = 0;
        bulletMgr[n].spd = spd;
        bulletMgr[n].minLiveTime = minLiveTime;
        bulletMgr[n].link = s;
        bulletMgr[n].linkNum = s ->bullets.size();
        s -> bullets.push_back(n);
        return n;
    }else return -1;
}

/* 仅对Boss符卡有效的API */
typedef void(*SCPartten)(Boss* b,int cnt,int scnt,int& image,double& boss_x,double& boss_y,double& boss_spd,double& boss_aspd,double& boss_angle,double hp,const std::vector<int>& bullets,Snow::Bundle<256>& data);
//Boss 指针，Boss总帧数，符卡生存帧数，使用中的Boss图像，Boss坐标x，Boss坐标y，Boss速度，Boss加速度，Boss移动角度，符卡生命剩余，子弹表
extern SCPartten scPartten [];

/* 通用API */
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
    bulletMgr[n].self_angle = 0;
    bulletMgr[n].spd = spd;
    bulletMgr[n].minLiveTime = minLiveTime;
    bulletMgr[n].link = nullptr;
    bulletMgr[n].linkNum = s ->m_bullets.size();
    s -> m_bullets.push_back(n);
    return n;
}


