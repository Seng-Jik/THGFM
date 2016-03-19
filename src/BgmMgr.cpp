#include "BgmMgr.h"
#include "Snow/Debug.h"
//#define DISABLE
static char memory_Dontuse [64];    //此处发生内存损坏，已修补
BgmMgr bgm;

Bgm::Bgm(const std::string& rv)
{
    Snow::ResVal rvo;
    rvo.Load(rv);
    Snow::ResFile r;
    if(rvo.Int("HAVE_HEAD")){
        r.Load(rvo.Str("BGM_HEAD"));
        head = Mix_LoadWAV_RW(r,r.Size());
        SDL_assert(head);
    }else head = nullptr;
    r.Free();
    r.Load(rvo.Str("BGM_LOOP"));
    loop = Mix_LoadWAV_RW(r,r.Size());
    SDL_assert(loop);
}

Bgm::~Bgm()
{
    if(head) Mix_FreeChunk(head);
    if(loop) Mix_FreeChunk(loop);
}


void BgmMgr::LoadMusic(const std::string& rv)
{
    Clear();
    m_bgm = new Bgm(rv);
    PNT("MUSIC RELOADED");
}

void BgmMgr::Clear()
{
    Stop(0);
    delete m_bgm;
    m_bgm = nullptr;
    PNT("MUSIC KILLED");
}

void BgmMgr::UseMusic(Bgm* bgm)
{
    Clear();
    m_bgm = bgm;
    PNT("MUSIC CHANGED.");
}


void BgmMgr::Play()
{
    #ifndef DISABLE
    if(m_bgm->head) Mix_PlayChannel(0,m_bgm->head,0);
    else Mix_PlayChannel(0,m_bgm->loop,-1);
    #endif // DISABLE
}


void BgmMgr::Stop(int ms)
{
    #ifndef DISABLE
    if(Mix_Playing(0)) Mix_FadeOutChannel(0,ms);
    #endif // DISABLE
}

void BgmMgr::Pause()
{
    //Mix_PauseMusic();
    #ifndef DISABLE
    Mix_Pause(0);
    #endif // DISABLE
}

void BgmMgr::Resume()
{
    //Mix_ResumeMusic();
    #ifndef DISABLE
    Mix_Resume(0);
    #endif // DISABLE
}
