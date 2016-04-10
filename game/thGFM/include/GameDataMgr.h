#pragma once
#include "Snow.h"
#include <bitset>

/** 存档管理器 */
//除特殊注明外，都需要Flush。

class GameDataMgr{
private:
    std::bitset<64> m_bools;
public:
    void ReadInitSetting(Uint8* gamepad);   //读取基本设置
    //gamepad设置为Uint8数组，共八个数字，前四个分别为1P的SHOT,BOOM,SLOW,PAUSE的手柄键值，后四个为2P的。
    void SetGamePad(const Uint8* gamepad);
    //格式同上，设置手柄映射并应用到文件，不需要手动调用Flush()。
    inline void SetBool(int i,bool b){m_bools[i]=b;}
    //设置一个bool值，存档中可以存储64个bool值。
    inline bool GetBool(int i){return m_bools[i];}
    //获取一个Bool值
    void Flush();
    //把内存中的存档写到文件
};

extern GameDataMgr gameData;

/** 数据文件格式 *
 * PAW02.sav
 * 前8字节为Uint8的手柄映射
 * 后跟8字节，为64位，每一位都是一个bool值
**/
