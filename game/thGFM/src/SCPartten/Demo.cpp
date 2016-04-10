#include "StdParttenAPI.h"
#include "ShotParttenAPI.h"

void SC000(Boss* b,int cnt,int scnt,int& image,double& boss_x,double& boss_y,double& boss_spd,double& boss_aspd,double& boss_angle,double hp,const std::vector<int>& bullets,Snow::Bundle<256>& data){
    SCCreateBlt(b,boss_x,boss_y,0,10,300,1);
    boss_spd = 5;
    boss_angle += 0.5;
}

void SC001(Boss* b,int cnt,int scnt,int& image,double& boss_x,double& boss_y,double& boss_spd,double& boss_aspd,double& boss_angle,double hp,const std::vector<int>& bullets,Snow::Bundle<256>& data){
    SCCreateBlt(b,boss_x,boss_y,0,10,300,0);
    boss_spd = 2;
    boss_angle += 0.1;
}

void SC002(Boss* b,int cnt,int scnt,int& image,double& boss_x,double& boss_y,double& boss_spd,double& boss_aspd,double& boss_angle,double hp,const std::vector<int>& bullets,Snow::Bundle<256>& data){
    SCCreateBlt(b,boss_x,boss_y,0,10,300,1);
    SCCreateBlt(b,boss_x,boss_y,0,10,300,0);
    boss_spd = 1;
    boss_angle += 0.05;
}

SCPartten scPartten [500]={
    &SC000,
    &SC001,
    &SC002
};

//编号规则
//第一位为难度，0-4分别为E,N,H,L,EX
//第二位为关卡
//第三位为符卡/非符编号
