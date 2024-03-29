#pragma once
#include "StageMgr.h"
#include "Player.h"
#include <cmath>
#include <cstdlib>
#include "Tools.h"
#include "Beater.h"

#define StdGetAngle(x1,y1,x2,y2) atan2(y1-y2,x1-x2);

inline void EneGetPos(int n,double& x,double& y){
    auto p = stage.GetEnemy(n);
    x = p -> x;
    y = p -> y;
}

inline void PlrGetPos(int playerNum,double& x,double& y){
    player[playerNum].GetPos(x,y);
}

inline double EneGetPlrAngle(int playerNum,int enemy){
    double x,y;
    player[playerNum].GetPos(x,y);
    return StdGetAngle(stage.GetEnemy(enemy) ->x,stage.GetEnemy(enemy) ->y,x,y);
}

inline int PlrCount(){
    return 1;
}

inline float Rand(){
    return float(rand()) / RAND_MAX;
}
