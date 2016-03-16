#include "StdParttenAPI.h"
#include "ShotParttenAPI.h"
#include "Snow/Debug.h"
#include "SeMgr.h"
#include "MathFunc.h"
//大妖精

void SC410(Boss* b,int cnt,int scnt,int& image,double& boss_x,double& boss_y,double& boss_spd,double& boss_aspd,double& boss_angle,double hp,const std::vector<int>& bullets,Snow::Bundle<256>& data){
    //飘下羽毛
    if(scnt % 2 == 0){
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

void SC411(Boss* b,int cnt,int scnt,int& image,double& boss_x,double& boss_y,double& boss_spd,double& boss_aspd,double& boss_angle,double hp,const std::vector<int>& bullets,Snow::Bundle<256>& data){
    //子弹加速度
    for(int i:bullets)
        if(BltState(i,0) == 1) BltSpd(i) += 0.1;
        else BltSpd(i) -= 0.1;

    //从大妖精位置发出彩色光芒
    int kcnt;

    struct DaiAI{
        bool ismoving;
        int trg_x,trg_y,o_x,o_y;
        int beg_cnt;
    }ai;
    if(scnt <= 1){
        kcnt = 0;
        ai.ismoving = true;
        ai.trg_x = 875;
        ai.trg_y = 262;
        ai.o_x = boss_x;
        ai.o_y = boss_y;
        ai.beg_cnt = scnt;
    }else{
        data.Read<int>(kcnt);
        data.Read<DaiAI>(ai);
        ++kcnt;
        if(beater.IsBeatFrame()  && beater.GetBeatNum()%4==0 && scnt>30) kcnt = 10;
        if(kcnt >= 10 && kcnt <= 50){
            for(int i = 0; i < 70;++i){
                double ang = Rand()*2*M_PI;
                int num = SCCreateBlt(b,boss_x+40,boss_y+40,ang,0.1,0,20+Rand()*8);
                BltSelfAngle(num) =ang;
                BltState(num,0) = 1;
            }
        }
        if(beater.IsBeatFrame()&& (beater.GetBeatNum()%4==1|| beater.GetBeatNum()%4==3)&& scnt>60){
            int st = 20+Rand()*8;
            for(int i = 0; i < 90;++i){
                double ang = 2*M_PI/90*i;
                int num = SCCreateBlt(b,boss_x+40,boss_y+40,ang,7,0,st);
                BltSelfAngle(num) =ang;
                BltState(num,0) = 2;
            }
        }
        //if(kcnt >= 200) kcnt = 10;

        if(ai.ismoving){
            float per = ACGCross::ArcFunc(float(scnt - ai.beg_cnt)/60);
            if(per >= 1 || per <= 0) {ai.ismoving = false;}
            else{
                boss_x = ai.o_x + (ai.trg_x-ai.o_x)*per;
                boss_y = ai.o_y+ (ai.trg_y-ai.o_y)*per;
            }
        }
    }
    data.ResetPtr();
    data.Write<int>(kcnt);
    data.Write<DaiAI>(ai);
}

void SC412(Boss* b,int cnt,int scnt,int& image,double& boss_x,double& boss_y,double& boss_spd,double& boss_aspd,double& boss_angle,double hp,const std::vector<int>& bullets,Snow::Bundle<256>& data){
    struct SC412Dmk{
        double dmk_speed;
    }dmk;
    data.Read<SC412Dmk>(dmk);

    //开场环状弹幕
    //0号状态：0为刚发射出来，正在急剧减速，1为减速完成，正在加速
    //1号状态，这个值越大就证明发射时越晚，越靠里

    //连续的狙
    //0号状态：10为不改变方向的弹幕，11为改变方向的弹幕，12为改变方向后的11
    //1号状态：对11状态起作用，距离改变方向时还剩余帧数
    if(scnt == 0 || scnt == 130){
        dmk.dmk_speed = 40;
    }
    if((scnt >= 1 && scnt <= 31) || (scnt >= 230 && scnt <= 260) || (scnt >= 455 && scnt <= 485) || (scnt >= 677 && scnt <= 707)){
        int style = 30;
        int tcnt = scnt - 460;
        if((scnt >= 1 && scnt <= 31) || (scnt >= 455 && scnt <= 485)){
            style = 29;
            tcnt = scnt - 1;
        }

        double source_angle = M_PI*2*tcnt/60;
        for(int j = 0;j < 8;++j){
            for(int i = 0;i < 10;++i){
                int blt = SCCreateBlt(b,boss_x+40,boss_y+40,source_angle,dmk.dmk_speed+0.5*i,0,style);
                BltSelfAngle(blt) = source_angle;
                BltState(blt,0)=0;
                BltState(blt,1) = i;
            }
            source_angle += M_PI/4;
        }
    }
    for(int i:bullets) if(BltState(i,0)==0){
        BltSpd(i) -= 4;
        if(BltSpd(i) <= 5) BltState(i,0) = 1;
    }else if(BltState(i,0)==1){
        BltSpd(i) += BltState(i,1)/500;
    }else if(BltState(i,0)==11){
        BltState(i,1) -= 1;
        if(BltState(i,1) == 0){
            double x,y;
            PlrGetPos(0,x,y);
            BltAngle(i) = BltSelfAngle(i) = StdGetAngle(bulletMgr[i].x,bulletMgr[i].y,x,y);
            BltState(i,0) = 12;
        }
    }

    //连续的狙
    if(scnt >= 935 && scnt%18==0){
        double x,y;
        PlrGetPos(0,x,y);
        double ang = StdGetAngle(boss_x+40,boss_y+40,x,y);
        for(int i = -3;i<=3;++i){
            int n = SCCreateBlt(b,boss_x+40,boss_y+40,ang - 0.2*i,5,0,23);
            BltSelfAngle(n) = ang - 0.2*i;
            BltState(n,0) = 10;

            n = SCCreateBlt(b,boss_x+40,boss_y+40,ang - 0.4*i,5,0,30);
            BltSelfAngle(n) = ang - 0.3*i;
            BltState(n,0) = 11;
            BltState(n,1) = 45;
        }
    }

    data.ResetPtr();
    data.Write<SC412Dmk>(dmk);
}

void ScParttenInit_Stage1L(){
    scPartten[410] = &SC410;
    scPartten[411] = &SC411;
    scPartten[412] = &SC412;
}

