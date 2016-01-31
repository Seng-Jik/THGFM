<<<<<<< HEAD
#include "BgmMgr.h"
#include "Snow/Debug.h"
BgmMgr bgm;

static void _whenMusicFinished(){
    Mix_SetMusicPosition(30);
}
void BgmMgr::LoadMusic(const std::string& s,int loops)
{
    Mix_HookMusicFinished(_whenMusicFinished);
    Clear();
    Snow::ResFile r;
    r.Load(s);
    //m_bgmMus = Mix_LoadMUS_RW(r,r.Size());
    /* Bug!!! */
    m_bgmMus = Mix_LoadMUS(s.c_str());
    PNT(Mix_GetError());
    m_loops = loops;
    SDL_assert(m_bgmMus != nullptr);

}

void BgmMgr::Clear()
{
    Mix_FreeMusic(m_bgmMus);
    m_bgmMus = nullptr;
}

void BgmMgr::Play()
{
    Mix_PlayMusic(m_bgmMus,m_loops);
}


void BgmMgr::Stop(int ms)
{
    Mix_FadeOutMusic(ms);
}

void BgmMgr::Pause()
{
    Mix_PauseMusic();
}

void BgmMgr::Resume()
{
    Mix_ResumeMusic();
}
=======
#include "BgmMgr.h"
#include "Snow/Debug.h"
BgmMgr bgm;

static void _whenMusicFinished(){
    Mix_SetMusicPosition(30);
}
void BgmMgr::LoadMusic(const std::string& s,int loops)
{
    Mix_HookMusicFinished(_whenMusicFinished);
    Clear();
    Snow::ResFile r;
    r.Load(s);
    //m_bgmMus = Mix_LoadMUS_RW(r,r.Size());
    /* Bug!!! */
    m_bgmMus = Mix_LoadMUS(s.c_str());
    PNT(Mix_GetError());
    m_loops = loops;
    SDL_assert(m_bgmMus != nullptr);

}

void BgmMgr::Clear()
{
    Mix_FreeMusic(m_bgmMus);
    m_bgmMus = nullptr;
}

void BgmMgr::Play()
{
    Mix_PlayMusic(m_bgmMus,m_loops);
}


void BgmMgr::Stop(int ms)
{
    Mix_FadeOutMusic(ms);
}

void BgmMgr::Pause()
{
    Mix_PauseMusic();
}

void BgmMgr::Resume()
{
    Mix_ResumeMusic();
}
>>>>>>> 720c0b59a97be74f02a7c85da646f3f62bece3a5
