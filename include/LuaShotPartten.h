#ifndef LUASHOTPARTTEN_H
#define LUASHOTPARTTEN_H
#include "lua/lua.hpp"
#include "Snow.h"
struct Shot;
class LuaShotParttenMgr
{
    public:
        void InitByStageMgr();   //��StageMgr�����ʼ��
        void ProcShot(Shot*,int enemy);

        void saveBulletVars();   //�洢�ӵ��ĸ���,��Ҫ��������
        void loadBulletVars(); //��ȡLua������,��Ҫ��������
    protected:
    private:

        lua_State* m_luaVM;
        Snow::ResFile* m_parttens[256];
};

extern LuaShotParttenMgr luaShotParttens;

#endif // LUASHOTPARTTEN_H
