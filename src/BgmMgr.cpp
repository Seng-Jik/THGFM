#include "BgmMgr.h"
#include "Snow/Debug.h"
//#define DISABLE
BgmMgr bgm;

static void _whenMusicFinished(){
    //Mix_SetMusicPosition(30);
}
void BgmMgr::LoadMusic(const std::string& s,int loops)
{
    //Mix_HookMusicFinished(_whenMusicFinished);
    Clear();
    Snow::ResFile r;
    r.Load(s);
    //m_bgmMus = Mix_LoadMUS_RW(r,r.Size());
    /* Bug!!! */
    //m_bgmMus = Mix_LoadMUS(s.c_str());
    m_bgmMus = Mix_LoadWAV_RW(r,r.Size());
    m_loops = loops;
    SDL_assert(m_bgmMus != nullptr);

}

void BgmMgr::UseMusic(Mix_Chunk* mix,int loops)
{
    Clear();
    m_bgmMus = mix;
    SDL_assert(mix != nullptr);
    m_loops = loops;
}


void BgmMgr::Clear()
{
    //Mix_FreeMusic(m_bgmMus);
    Mix_HaltChannel(0);
    Mix_FreeChunk(m_bgmMus);
    m_bgmMus = nullptr;
}

void BgmMgr::Play()
{
    #ifndef DISABLE
    Mix_PlayChannel(0,m_bgmMus,m_loops);
    while(!Mix_Playing(0));
    #endif // DISABLE
}


void BgmMgr::Stop(int ms)
{
    //Mix_FadeOutMusic(ms);
    #ifndef DISABLE
    Mix_FadeOutChannel(0,ms);
    #endif // DISABLE
}

void BgmMgr::Pause()
{
    //Mix_PauseMusic();
    Mix_Pause(0);
}

void BgmMgr::Resume()
{
    //Mix_ResumeMusic();
    #ifndef DISABLE
    Mix_Resume(0);
    while(!Mix_Playing(0));
    #endif // DISABLE
}
