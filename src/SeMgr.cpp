#include "SeMgr.h"
Mix_Chunk* SeMgr::m_se[20] = {nullptr};
SeMgr se;
void SeMgr::Init()
{
    loadSe(DEMOSE,"Se/0.wav");
}

void SeMgr::Play(SeName n)
{
    Mix_PlayChannel(n+1,m_se[n],0);
}

void SeMgr::Pause()
{
    for(int i = 0;i < 20;++i){
        if(Mix_Playing(i+1) && m_se[i] != nullptr) Mix_Pause(i+1);
    }
}

void SeMgr::Resume()
{
    for(int i = 0;i < 20;++i){
        if(Mix_Paused(i+1) && m_se[i] != nullptr) Mix_Resume(i+1);
    }
}


void SeMgr::loadSe(SeName n, const std::string& se)
{
    Snow::ResFile r;
    r.Load(se);
    m_se[n] = Mix_LoadWAV_RW(r,r.Size());
}
