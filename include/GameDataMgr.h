#pragma once
#include "Snow.h"

/** 存档管理器 */

class GameDataMgr{
public:
    void ReadInitSetting(Uint8* gamepad);   //读取基本设置
    //gamepad设置为Uint8数组，共八个数字，前四个分别为1P的SHOT,BOOM,SLOW,PAUSE的手柄键值，后四个为2P的。
    void SetGamePad(const Uint8* gamepad);
    //格式同上，设置手柄映射并应用到文件
};

extern GameDataMgr gameData;

/** 数据文件格式 *
 * PAW02.sav
 * 前8字节为Uint8的手柄映射
 * 
**/