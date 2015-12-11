#include "StageMgr.h"
#include "BulletMgr.h"
typedef void(*ShotPartten)(Shot*,int);
extern ShotPartten shotPartten [];

#define ShtBullets(s) s -> bullets
#define ShtEnd(s) s -> live = Shot::STOPLIVE
#define ShtArgs(s) s -> parttenArgs

inline int ShtCnt(Shot* s){return s -> cnt;}
inline void ShtStop(Shot* s){s -> live = Shot::STOPLIVE;}

#define BltAngle(b) bulletMgr[b].angle
#define BltSpd(b) bulletMgr[b].spd
#define BltState(b,i) bulletMgr[b].state[i]
#include "Snow/Debug.h"
static inline int ShtCreateBlt(Shot* s,double x,double y,double angle,double spd,int minLiveTime,int style)
{
    if(s->live == Shot::LIVE){
        int n = bulletMgr.Alloc();
        if(n == -1) return -1;
        bulletMgr[n].x = x;
        bulletMgr[n].y = y;
        bulletMgr[n].angle = angle;
        bulletMgr[n].spd = spd;
        bulletMgr[n].minLiveTime = minLiveTime;
        bulletMgr[n].style = style;
        bulletMgr[n].link = s;
        bulletMgr[n].linkNum = s ->bullets.size();
        s -> bullets.push_back(n);
        return n;
    }else return -1;
}


