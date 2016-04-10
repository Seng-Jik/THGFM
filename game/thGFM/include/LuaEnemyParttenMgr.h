#ifndef LUAENEMYPARTTENMGR_H
#define LUAENEMYPARTTENMGR_H
#include "liblua.h"
#include "Snow.h"

struct Enemy;

class LuaEnemyParttenMgr
{
    public:
        void InitByStageMgr();   //由StageMgr负责初始化
        void ProcEnemy(Enemy*);
    protected:
    private:
        lua_State* m_luaVM;
        Snow::ResFile* m_parttens[256];
};

extern LuaEnemyParttenMgr luaEnemyParttens;

#endif // LUAENEMYPARTTENMGR_H
