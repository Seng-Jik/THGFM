#include "LuaEnemyParttenMgr.h"
#include "Snow/Debug.h"
#include "StageMgr.h"
#include <string>

LuaEnemyParttenMgr luaEnemyParttens;

void LuaEnemyParttenMgr::InitByStageMgr()
{
    m_luaVM = luaL_newstate();
    luaopen_base(m_luaVM);
    luaopen_io(m_luaVM);
    luaopen_math(m_luaVM);
    luaopen_debug(m_luaVM);
    Snow::CSVReader csv;
    csv.LoadCSV("EnemyParttens/parttens.csv");
    int i = 0;
    csv.NextLine();
    do{
        std::string path;
        csv.PopStr(path);
        if(!path.empty()){
            m_parttens[i] = new Snow::ResFile;
            m_parttens[i] -> Load("EnemyParttens/"+path);
        }
        ++i;
    }while(csv.NextLine());
}

#include "StdParttenAPI.h"
void LuaEnemyParttenMgr::ProcEnemy(Enemy* e)
{

    // 只读组数据
    lua_pushnumber(m_luaVM,e -> cnt);
    lua_pushnumber(m_luaVM,e -> num);
    lua_pushnumber(m_luaVM,stage.GetLv());
    lua_setglobal(m_luaVM,"level");
    lua_setglobal(m_luaVM,"num");
    lua_setglobal(m_luaVM,"cnt");

    lua_pushnumber(m_luaVM,e -> x);
    lua_pushnumber(m_luaVM,e -> y);
    lua_pushnumber(m_luaVM,e -> hp);
    lua_setglobal(m_luaVM,"hp");
    lua_setglobal(m_luaVM,"y");
    lua_setglobal(m_luaVM,"x");

    lua_pushboolean(m_luaVM,beater.IsBeatFrame());
    lua_pushnumber(m_luaVM,beater.GetBeatNum());
    lua_setglobal(m_luaVM,"beatCount");
    lua_setglobal(m_luaVM,"beatFrame");


    // 可写组数据
    lua_pushnumber(m_luaVM,e -> angle);
    lua_pushnumber(m_luaVM,e -> spd);
    lua_setglobal(m_luaVM,"spd");
    lua_setglobal(m_luaVM,"angle");
    for(int i = 0;i < 8;++i)
        lua_pushnumber(m_luaVM,e ->parttenArgs[i]);
    lua_setglobal(m_luaVM,"m7");
    lua_setglobal(m_luaVM,"m6");
    lua_setglobal(m_luaVM,"m5");
    lua_setglobal(m_luaVM,"m4");
    lua_setglobal(m_luaVM,"m3");
    lua_setglobal(m_luaVM,"m2");
    lua_setglobal(m_luaVM,"m1");
    lua_setglobal(m_luaVM,"m0");

    //PNT("CALL LUA");
    luaL_loadbuffer(m_luaVM,*m_parttens[e->partten-1],m_parttens[e->partten-1]->Size(),"ep");
    if(lua_pcall(m_luaVM,0,0,0)){
        PNT("Lua Enemy Partten Error in partten"<<e->partten-1);
        PNT(lua_tostring(m_luaVM,-1));
        system("pause");
        exit(-1);
    }

    // 可写组数据回写
    lua_getglobal(m_luaVM,"m7");
    lua_getglobal(m_luaVM,"m6");
    lua_getglobal(m_luaVM,"m5");
    lua_getglobal(m_luaVM,"m4");
    lua_getglobal(m_luaVM,"m3");
    lua_getglobal(m_luaVM,"m2");
    lua_getglobal(m_luaVM,"m1");
    lua_getglobal(m_luaVM,"m0");
    for(int i = 0;i < 8;++i){
        e ->parttenArgs[i] = lua_tonumber(m_luaVM,-1);
        lua_pop(m_luaVM,1);
    }

    lua_getglobal(m_luaVM,"spd");
    lua_getglobal(m_luaVM,"angle");
    e -> angle = lua_tonumber(m_luaVM,-1);
    e -> spd = lua_tonumber(m_luaVM,-2);
    lua_pop(m_luaVM,2);
}
