#include "ShotParttenAPI.h"
#include "StdParttenAPI.h"
#include <Snow/Bundle.h>
#include <iostream>
#include <cstdlib>

static void L100(Shot* s,int enemy){
    if(beater.IsBeatFrame()){
    //if(ShtCnt(s)%1== 1){// && ShtCnt(s) <= 3200){
        double x,y;
        EneGetPos(enemy,x,y);
        //ShtCreateBlt(s,x,y,EneGetPlrAngle(0,enemy),0.1,30,0);
        ShtCreateBlt(s,1000,500,EneGetPlrAngle(0,enemy),4,0,1);
        ShtEnd();
    }
    ShtForBullet(i){
        //BltSpd(*p) = 0.01;
        //BltAngle(*p) += 0.05;
        //if(beater.IsBeatFrame()) BltSpd(*p) = -BltSpd(*p);
        BltSpd(i) += 0.001;
        //BltSelfAngle(i) = BltAngle(i) = EneGetPlrAngle(0,enemy);
        //BltAngle(*p) = EneGetPlrAngle(0,enemy);
    }
}

ShotPartten shotPartten [4000] = {nullptr};
//函数命名规则：
//第一个字母代表等级，第一个数字代表关卡，特别的，EX关卡为EX00开始。
//后两位数字为敌人模式编号。
//排序规则：
//除了第一个字母，E=0,N=1,H=2,L=3
//其余一致。
//特别的，EX前两位为00

//在这个函数里创建弹幕运动模式列表

void InitShtPartten_1L(){
    shotPartten [3100] = &L100;
}
