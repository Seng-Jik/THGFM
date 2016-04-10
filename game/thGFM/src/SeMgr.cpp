#include "SeMgr.h"
Mix_Chunk* SeMgr::m_se[50] = {nullptr};
SeMgr se;
std::map<std::string,Mix_Chunk*> SeMgr::m_map;
void SeMgr::Init()
{
    loadSe(BONUS,"bonus");
    loadSe(BONUS2,"bonus2");
    loadSe(CANCEL00,"cancel00");
    loadSe(CARDGET,"cardget");
    loadSe(CAT00,"cat00");
    loadSe(CAT01,"cat01");
    loadSe(CH00,"ch00");
    loadSe(CH01,"ch01");
    loadSe(DAMAGE00,"damage00");
    loadSe(DAMAGE01,"damage01");
    loadSe(ENEP00,"enep00");
    loadSe(ENEP01,"enep01");
    loadSe(ENEP02,"enep02");
    loadSe(EXTEND,"extend");
    loadSe(GRAZE,"graze");
    loadSe(GUN00,"gun00");
    loadSe(HINT00,"hint00");
    loadSe(INVALID,"invalid");
    loadSe(ITEM00,"item00");
    loadSe(KIRA00,"kira00");
    loadSe(KIRA01,"kira01");
    loadSe(KIRA02,"kira02");
    loadSe(LAZER00,"lazer00");
    loadSe(LAZER01,"lazer01");
    loadSe(MSL,"msl");
    loadSe(NEP00,"nep00");
    loadSe(OK00,"ok00");
    loadSe(OPTION,"option");
    loadSe(PAUSE,"pause");
    loadSe(PLDEAD00,"pldead00");
    loadSe(PLST00,"plst00");
    loadSe(POWER0,"power0");
    loadSe(POWER1,"power1");
    loadSe(POWERUP,"powerup");
    loadSe(SELECT00,"select00");
    loadSe(SLASH,"slash");
    loadSe(TAN00,"tan00");
    loadSe(TAN01,"tan01");
    loadSe(TAN02,"tan02");
    loadSe(TIMEOUT,"timeout");
    loadSe(TIMEOUT2,"timeout2");
    loadSe(WARPL,"warpl");
    loadSe(WARPR,"warpr");
    loadSe(WATER,"water");
    /*for(int i = 1;i < 100;++i){
        Mix_Volume(i,50);
    }*/
}

void SeMgr::Play(SeName n)
{
    Mix_PlayChannel(-1,m_se[n],0);
}

void SeMgr::Play(const std::string& s)
{
    Mix_PlayChannel(-1,m_map[s],0);
}


void SeMgr::Pause()
{
}

void SeMgr::Resume()
{
}


void SeMgr::loadSe(SeName n, const std::string& se)
{
    Snow::ResFile r;
    r.Load("Se/se_"+se+".WAV");
    m_se[n] = Mix_LoadWAV_RW(r,r.Size());
    m_map[se] = m_se[n];
    SDL_assert(m_se[n]);
}
