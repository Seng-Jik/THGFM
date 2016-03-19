#include "EnemyParttenAPI.h"
#include "EnePartten.h"
#include "StdParttenAPI.h"

static void L100(Enemy* e,int n){
    if(EneCnt(e)>=50 && EneCnt(e)<=100){
        if(n%2) EneAngle(e) += 0.1;
        else EneAngle(e) -= 0.1;
        EneSpd(e) -= 0.2;
    }
    if(EneCnt(e) == 100) EneSpd(e) = 0;
    //if(EneCnt(e) >= 200 && EneCnt(e) <= 260)
       // EneSpd(e) += 0.2;
}

//函数命名规则：
//第一个字母代表等级，第一个数字代表关卡，特别的，EX关卡为EX00开始。
//后两位数字为敌人模式编号。
//排序规则：
//除了第一个字母，E=0,N=1,H=2,L=3
//其余一致。
//特别的，EX前两位为00
//必须为static函数

//在这个函数里创建敌人运动模式列表
EnemyPartten enemyPartten[4000];
void InitEnePartten_1L(){
    enemyPartten[3100] = &L100;

}
