#include "LuaShotPartten.h"
#include "StageMgr.h"
#include <vector>
#include "ShotParttenAPI.h"
#include "StdParttenAPI.h"

LuaShotParttenMgr luaShotParttens;

//Lua交互API
static Shot* now;
static int nowBullet = -1;   //全局的子弹编号
static std::vector<int>::iterator nowBullet_inThisShot;    //射击集中的当前子弹

static int luaApi_NextBullet(lua_State* s){
    luaShotParttens.saveBulletVars();
    int ret;
    do{
        ++nowBullet_inThisShot;
        if(nowBullet_inThisShot == now->bullets.end()){
            ret = -1;
            break;
        }
        else{
            nowBullet = *nowBullet_inThisShot;
            ret = 0;
        }
    }while(nowBullet == -1);
    lua_pushnumber(s,ret);
    luaShotParttens.loadBulletVars();
    return 1;
}

static int luaApi_SelectFirstBullet(lua_State* s){
    luaShotParttens.saveBulletVars();
    int ret;
    nowBullet_inThisShot = now ->bullets.begin();
    nowBullet = *nowBullet_inThisShot;
    if(nowBullet_inThisShot == now->bullets.end()){
        lua_pushnumber(s,-1);
        return 1;
    }
    while(nowBullet == -1){
        ++nowBullet_inThisShot;
        if(nowBullet_inThisShot == now->bullets.end()){
            ret = -1;
            break;
        }
        else{
            nowBullet = *nowBullet_inThisShot;
            ret = 0;
        }
    }
    lua_pushnumber(s,ret);
    luaShotParttens.loadBulletVars();
    return 1;
}

static int luaApi_ShootBullet(lua_State* s){
    luaShotParttens.saveBulletVars();
    float x = lua_tonumber(s,1);
    float y = lua_tonumber(s,2);
    float angle = lua_tonumber(s,3);
    float spd = lua_tonumber(s,4);
    int minLive = lua_tonumber(s,5);
    int style = lua_tonumber(s,6);
    nowBullet = ShtCreateBlt(now,x,y,angle,spd,minLive,style);
    if(-1 == nowBullet){
        lua_pushnumber(s,-1);
    }else lua_pushnumber(s,0);
    luaShotParttens.loadBulletVars();
    return 1;
}

static int luaApi_KillBullet(lua_State*){
    if(nowBullet != -1) bulletMgr.KillBulletAndInstallEffect(nowBullet);
    return 0;
}

static int luaApi_ShtEnd(lua_State*){
    auto s = now;
    ShtEnd();
    return 0;
}

static int luaApi_Rand(lua_State* s){
    lua_pushnumber(s,Rand());
    return 1;
}

//Shot Data系API
static int luaApi_GetDataSize(lua_State* s){
    lua_pushnumber(s,now->parttenArgs.size());
    return 1;
}

static int luaApi_MoreData(lua_State*){
    now->parttenArgs.push_back(0);
    return 0;
}

static int luaApi_SetData(lua_State* s){
    now->parttenArgs[lua_tonumber(s,1)] = lua_tonumber(s,2);
    return 0;
}

static int luaApi_GetData(lua_State* s){
    lua_pushnumber(s,now->parttenArgs[lua_tonumber(s,1)]);
    return 1;
}

void LuaShotParttenMgr::InitByStageMgr()
{
    m_luaVM = luaL_newstate();
    luaopen_base(m_luaVM);
    luaopen_io(m_luaVM);
    luaopen_math(m_luaVM);
    luaopen_debug(m_luaVM);

    lua_register(m_luaVM,"Rand",&luaApi_Rand);
    lua_register(m_luaVM,"ShtEnd",&luaApi_ShtEnd);
    lua_register(m_luaVM,"KillBullet",&luaApi_KillBullet);
    lua_register(m_luaVM,"ShootBullet",&luaApi_ShootBullet);
    lua_register(m_luaVM,"SelectFirstBullet",&luaApi_SelectFirstBullet);
    lua_register(m_luaVM,"NextBullet",&luaApi_NextBullet);

    lua_register(m_luaVM,"GetDataSize",&luaApi_GetDataSize);
    lua_register(m_luaVM,"GetData",&luaApi_GetData);
    lua_register(m_luaVM,"SetData",&luaApi_SetData);
    lua_register(m_luaVM,"MoreData",&luaApi_MoreData);

    Snow::CSVReader csv;
    csv.LoadCSV("ShotParttens/parttens.csv");
    int i = 0;
    csv.NextLine();
    do{
        std::string path;
        csv.PopStr(path);
        if(!path.empty()){
            m_parttens[i] = new Snow::ResFile;
            m_parttens[i] -> Load("ShotParttens/"+path);
            PNT("LOAD SHOT PARTTEN  "<<m_parttens[i]->Size()<<"  ShotParttens/"+path);
        }
        ++i;
    }while(csv.NextLine());
}

void LuaShotParttenMgr::ProcShot(Shot* s,int enemy)
{
    now = s;
    nowBullet = -1;
    lua_pushnumber(m_luaVM,s->cnt);

    double ex,ey;
    EneGetPos(enemy,ex,ey);
    lua_pushnumber(m_luaVM,ex);
    lua_pushnumber(m_luaVM,ey);

    PlrGetPos(0,ex,ey);
    lua_pushnumber(m_luaVM,ex);
    lua_pushnumber(m_luaVM,ey);

    lua_pushnumber(m_luaVM,EneGetPlrAngle(0,enemy));
    lua_pushnumber(m_luaVM,stage.GetLv());

    lua_pushboolean(m_luaVM,beater.IsBeatFrame());
    lua_pushnumber(m_luaVM,beater.GetBeatNum());
    lua_setglobal(m_luaVM,"beatCount");
    lua_setglobal(m_luaVM,"beatFrame");

    lua_setglobal(m_luaVM,"level");
    lua_setglobal(m_luaVM,"ene_plr_Angle");
    lua_setglobal(m_luaVM,"plrY");
    lua_setglobal(m_luaVM,"plrX");
    lua_setglobal(m_luaVM,"eneY");
    lua_setglobal(m_luaVM,"eneX");
    lua_setglobal(m_luaVM,"shtFrame");

    lua_pushnumber(m_luaVM,s->partten);
    lua_setglobal(m_luaVM,"shtPartten");

    luaL_loadbuffer(m_luaVM,*m_parttens[s->partten-1],m_parttens[s->partten-1]->Size(),"sp");
    if(lua_pcall(m_luaVM,0,0,0)){
        PNT("Lua Shot Partten Error in partten"<<s->partten-1);
        PNT(lua_tostring(m_luaVM,-1));
        system("pause");
        exit(-1);
    }
    saveBulletVars();

    lua_getglobal(m_luaVM,"shtPartten");
    s->partten = lua_tonumber(m_luaVM,-1);
    lua_pop(m_luaVM,1);
}

void LuaShotParttenMgr::saveBulletVars()
{
    if(nowBullet == -1) return;
    Bullet& b = bulletMgr[nowBullet];
    lua_getglobal(m_luaVM,"bltSelfAngle");
    lua_getglobal(m_luaVM,"bltSpd");
    lua_getglobal(m_luaVM,"bltAngle");
    b.angle = lua_tonumber(m_luaVM,-1);
    b.spd = lua_tonumber(m_luaVM,-2);
    b.self_angle = lua_tonumber(m_luaVM,-3);
    lua_pop(m_luaVM,3);

    lua_getglobal(m_luaVM,"bltState7");
    lua_getglobal(m_luaVM,"bltState6");
    lua_getglobal(m_luaVM,"bltState5");
    lua_getglobal(m_luaVM,"bltState4");
    lua_getglobal(m_luaVM,"bltState3");
    lua_getglobal(m_luaVM,"bltState2");
    lua_getglobal(m_luaVM,"bltState1");
    lua_getglobal(m_luaVM,"bltState0");
    for(int i = 0;i < 8;++i){
        now ->parttenArgs[i] = lua_tonumber(m_luaVM,-1);
        lua_pop(m_luaVM,1);
    }
}

void LuaShotParttenMgr::loadBulletVars()
{
    if(nowBullet == -1) return;
    Bullet& b = bulletMgr[nowBullet];
    lua_pushnumber(m_luaVM,b.cnt);
    lua_setglobal(m_luaVM,"bltFrame");

    lua_pushnumber(m_luaVM,b.angle);
    lua_pushnumber(m_luaVM,b.spd);
    lua_pushnumber(m_luaVM,b.self_angle);
    lua_setglobal(m_luaVM,"bltSelfAngle");
    lua_setglobal(m_luaVM,"bltSpd");
    lua_setglobal(m_luaVM,"bltAngle");

    for(int i = 0;i < 8;++i)
        lua_pushnumber(m_luaVM,now ->parttenArgs[i]);
    lua_setglobal(m_luaVM,"bltState7");
    lua_setglobal(m_luaVM,"bltState6");
    lua_setglobal(m_luaVM,"bltState5");
    lua_setglobal(m_luaVM,"bltState4");
    lua_setglobal(m_luaVM,"bltState3");
    lua_setglobal(m_luaVM,"bltState2");
    lua_setglobal(m_luaVM,"bltState1");
    lua_setglobal(m_luaVM,"bltState0");
}
