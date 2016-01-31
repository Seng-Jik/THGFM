<<<<<<< HEAD
#include "EnemyParttenAPI.h"
#include "EnePartten.h"
#include "StdParttenAPI.h"

void ED000(Enemy* e,int n){
    if(EneCnt(e)>=50 && EneCnt(e)<=100){
        if(n%2) EneAngle(e) += 0.1;
        else EneAngle(e) -= 0.1;
        EneSpd(e) -= 0.2;
    }
    if(EneCnt(e) == 100) EneSpd(e) = 0;
    //if(EneCnt(e) >= 200 && EneCnt(e) <= 260)
       // EneSpd(e) += 0.2;
}

EnemyPartten enemyPartten[]= {
    &ED000
};
=======
#include "EnemyParttenAPI.h"
#include "EnePartten.h"
#include "StdParttenAPI.h"

void ED000(Enemy* e,int n){
    if(EneCnt(e)>=50 && EneCnt(e)<=100){
        if(n%2) EneAngle(e) += 0.1;
        else EneAngle(e) -= 0.1;
        EneSpd(e) -= 0.2;
    }
    if(EneCnt(e) == 100) EneSpd(e) = 0;
    //if(EneCnt(e) >= 200 && EneCnt(e) <= 260)
       // EneSpd(e) += 0.2;
}

EnemyPartten enemyPartten[]= {
    &ED000
};
>>>>>>> 720c0b59a97be74f02a7c85da646f3f62bece3a5
