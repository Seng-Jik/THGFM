#include "ShotParttenAPI.h"
#include "StdParttenAPI.h"
#include <iostream>
#include <cstdlib>
using namespace std;
void SD000(Shot* s,int enemy){
    if(ShtCnt(s)%2 == 1 && ShtCnt(s) <= 3200){
        double x,y;
        EneGetPos(enemy,x,y);
        //ShtCreateBlt(s,x,y,EneGetPlrAngle(0,enemy),0.1,30,0);
        ShtCreateBlt(s,800+rand()%200,rand()%576,EneGetPlrAngle(0,enemy),3,5,0);
    }
    FOR_EACH(p,s->bullets.begin(),s->bullets.end()){
        //BltSpd(*p) += 0.02;
        bulletMgr[*p].render_angle-=1;
        //BltAngle(*p) = EneGetPlrAngle(0,enemy);
    }
}

void SD001(Shot* s,int enemy){
    if(ShtCnt(s) >= 60) ShtStop(s);
    if(ShtCnt(s)%5 == 1){
        double x,y;
        EneGetPos(enemy,x,y);
        ShtCreateBlt(s,x,y,EneGetPlrAngle(0,enemy),3,3,0);
        //ShtCreateBlt(s,800+rand()%200,rand()%576,EneGetPlrAngle(0,enemy),0.1,30,0);
    }
    FOR_EACH(p,s->bullets.begin(),s->bullets.end()){
        if(BltSpd(*p) >= 5) BltSpd(*p) += 0.1;
        bulletMgr[*p].render_angle+=1;
        //BltAngle(*p) = EneGetPlrAngle(0,enemy);
    }
}

ShotPartten shotPartten [] = {
    &SD000,
    &SD001
};
