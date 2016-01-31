<<<<<<< HEAD
#include "ShotParttenAPI.h"
#include "StdParttenAPI.h"
#include <iostream>
#include <cstdlib>
using namespace std;
void SD000(Shot* s,int enemy){
    //if(ShtCnt(s)%1== 1){// && ShtCnt(s) <= 3200){
        double x,y;
        EneGetPos(enemy,x,y);
        //ShtCreateBlt(s,x,y,EneGetPlrAngle(0,enemy),0.1,30,0);
        ShtCreateBlt(s,800+rand()%200,rand()%720,EneGetPlrAngle(0,enemy),1,10,0);
   // }
    ShtForBullet(i){
        //BltSpd(*p) = 0.01;
        //BltAngle(*p) += 0.05;
        //if(beater.IsBeatFrame()) BltSpd(*p) = -BltSpd(*p);
        BltSpd(i) += 0.001;
        BltAngle(i) = EneGetPlrAngle(0,enemy);
        //BltAngle(*p) = EneGetPlrAngle(0,enemy);
    }
}

void SD001(Shot* s,int enemy){
    //if(ShtCnt(s) >= 60) ShtStop(s);
    //if(ShtCnt(s)%1 == 1){
        double x,y;
        EneGetPos(enemy,x,y);
        ShtCreateBlt(s,x,y,EneGetPlrAngle(0,enemy),2,400,0);
        //ShtCreateBlt(s,800+rand()%200,rand()%576,EneGetPlrAngle(0,enemy),0.1,30,0);
    //}
    ShtForBullet(i){
        //if(BltSpd(*p) >= 5) BltSpd(*p) += 0.1;
        //if(BltSpd(*p)>=3) BltSpd(*p) -= 0.1;
        //if(beater.IsBeatFrame()) BltSpd(*p) = -BltSpd(*p);
        BltSelfAngle(i)+=1;
        BltAngle(i) = EneGetPlrAngle(0,enemy);
        BltSpd(i) += 0.2;
        //BltAngle(*p) += 0.05;
        //BltAngle(*p) = EneGetPlrAngle(0,enemy);
    }
}

void SD002(Shot* s,int enemy){
    if(ShtCnt() <= 10){
        double x,y;
        EneGetPos(enemy,x,y);
        ShtCreateBlt(s,x,y,EneGetPlrAngle(0,enemy),5,500,0);
    }
}

void SD003(Shot* s,int enemy){
    static double dbl = 0;
    if(ShtCnt()%1 == 0 && ShtCnt()>=200){
        double x,y;
        EneGetPos(enemy,x,y);
        int b = ShtCreateBlt(s,x,y,EneGetPlrAngle(0,enemy),10,500,1);
        BltSelfAngle(b) = BltAngle(b) = dbl;
        dbl += 0.5;
        //PNT("CREATE BULLET"<<b);
    }
}

void SD004(Shot* s,int enemy){
    if(beater.IsBeatFrame()){
        double x,y;
        EneGetPos(enemy,x,y);
        int b = ShtCreateBlt(s,x,y,EneGetPlrAngle(0,enemy),5,500,0);
        BltSelfAngle(b) = EneGetPlrAngle(0,enemy);
    }
}

ShotPartten shotPartten [] = {
    &SD000,
    &SD001,
    &SD002,
    &SD003,
    &SD004
};
=======
#include "ShotParttenAPI.h"
#include "StdParttenAPI.h"
#include <iostream>
#include <cstdlib>
using namespace std;
void SD000(Shot* s,int enemy){
    //if(ShtCnt(s)%1== 1){// && ShtCnt(s) <= 3200){
        double x,y;
        EneGetPos(enemy,x,y);
        //ShtCreateBlt(s,x,y,EneGetPlrAngle(0,enemy),0.1,30,0);
        ShtCreateBlt(s,800+rand()%200,rand()%720,EneGetPlrAngle(0,enemy),1,10,0);
   // }
    ShtForBullet(i){
        //BltSpd(*p) = 0.01;
        //BltAngle(*p) += 0.05;
        //if(beater.IsBeatFrame()) BltSpd(*p) = -BltSpd(*p);
        BltSpd(i) += 0.001;
        BltAngle(i) = EneGetPlrAngle(0,enemy);
        //BltAngle(*p) = EneGetPlrAngle(0,enemy);
    }
}

void SD001(Shot* s,int enemy){
    //if(ShtCnt(s) >= 60) ShtStop(s);
    //if(ShtCnt(s)%1 == 1){
        double x,y;
        EneGetPos(enemy,x,y);
        ShtCreateBlt(s,x,y,EneGetPlrAngle(0,enemy),2,400,0);
        //ShtCreateBlt(s,800+rand()%200,rand()%576,EneGetPlrAngle(0,enemy),0.1,30,0);
    //}
    ShtForBullet(i){
        //if(BltSpd(*p) >= 5) BltSpd(*p) += 0.1;
        //if(BltSpd(*p)>=3) BltSpd(*p) -= 0.1;
        //if(beater.IsBeatFrame()) BltSpd(*p) = -BltSpd(*p);
        BltSelfAngle(i)+=1;
        BltAngle(i) = EneGetPlrAngle(0,enemy);
        BltSpd(i) += 0.2;
        //BltAngle(*p) += 0.05;
        //BltAngle(*p) = EneGetPlrAngle(0,enemy);
    }
}

void SD002(Shot* s,int enemy){
    if(ShtCnt() <= 10){
        double x,y;
        EneGetPos(enemy,x,y);
        ShtCreateBlt(s,x,y,EneGetPlrAngle(0,enemy),5,500,0);
    }
}

void SD003(Shot* s,int enemy){
    static double dbl = 0;
    if(ShtCnt()%1 == 0 && ShtCnt()>=200){
        double x,y;
        EneGetPos(enemy,x,y);
        int b = ShtCreateBlt(s,x,y,EneGetPlrAngle(0,enemy),10,500,1);
        BltSelfAngle(b) = BltAngle(b) = dbl;
        dbl += 0.5;
        //PNT("CREATE BULLET"<<b);
    }
}

void SD004(Shot* s,int enemy){
    if(beater.IsBeatFrame()){
        double x,y;
        EneGetPos(enemy,x,y);
        int b = ShtCreateBlt(s,x,y,EneGetPlrAngle(0,enemy),5,500,0);
        BltSelfAngle(b) = EneGetPlrAngle(0,enemy);
    }
}

ShotPartten shotPartten [] = {
    &SD000,
    &SD001,
    &SD002,
    &SD003,
    &SD004
};
>>>>>>> 720c0b59a97be74f02a7c85da646f3f62bece3a5
