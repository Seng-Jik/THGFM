#ifndef LUASHOTPARTTEN_H
#define LUASHOTPARTTEN_H
#include "lua/lua.hpp"
#include "Snow.h"
struct Shot;
class LuaShotParttenMgr
{
    public:
        void InitByStageMgr();   //由StageMgr负责初始化
        void ProcShot(Shot*,int enemy);

        void saveBulletVars();   //存储子弹的更改,不要在外界调用
        void loadBulletVars(); //获取Lua的设置,不要在外界调用
    protected:
    private:

        lua_State* m_luaVM;
        Snow::ResFile* m_parttens[256];
};

extern LuaShotParttenMgr luaShotParttens;

#endif // LUASHOTPARTTEN_H
