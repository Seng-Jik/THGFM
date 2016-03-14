#include "StdParttenAPI.h"
#include "ShotParttenAPI.h"
#include "Snow/Debug.h"
#include "SeMgr.h"
#include "MathFunc.h"
//大妖精

void SC410(Boss* b,int cnt,int scnt,int& image,double& boss_x,double& boss_y,double& boss_spd,double& boss_aspd,double& boss_angle,double hp,const std::vector<int>& bullets,Snow::Bundle<256>& data){
    //飘下羽毛
    if(scnt % 1 == 0){
        BltSelfAngle(SCCreateBlt(b,Rand()*WIDTH,-32,Rand()*M_PI+M_PI,Rand()*10,300,2)) = Rand()*2*M_PI;
    }
    boss_spd = 0;


    //大妖精躲闪AI
    //逃往目标位置型
    struct DaiAI{
        int trg_x,trg_y;
        int o_x,o_y;
        int beg_scnt;
        bool moving;
    }ai;
    if(scnt%300 == 0 || scnt <= 1){
        //选定目标点并准备前往
        ai.trg_x = Rand() * 200 + WIDTH - 350;
        ai.trg_y = Rand() * HEIGHT*0.8;
        ai.o_x = boss_x;
        ai.o_y = boss_y;
        ai.moving = true;
        ai.beg_scnt = scnt;
        data.Write<DaiAI>(ai);
    }else{
        data.Read<DaiAI>(ai);
        //前往途中
        if(!ai.moving) return;
        float per = ACGCross::SArcFunc(float(scnt-ai.beg_scnt) / 600);
        if(per >= 1 || per <= 0) {ai.moving = false;return;}
        boss_x = ai.o_x + (ai.trg_x-ai.o_x)*per;
        boss_y = ai.o_y+ (ai.trg_y-ai.o_y)*per;
    }

}

void ScParttenInit_Stage1L(){
    scPartten[410] = &SC410;
}

