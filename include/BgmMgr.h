#ifndef BGMMGR_H
#define BGMMGR_H
#include <string>
#include "Snow.h"
#include <atomic>
class BgmMgr
{
    public:
        void LoadMusic(const std::string&,int loops);
        void UseMusic(Mix_Chunk*,int loops);
        void Play();
        void Clear();
        void Stop(int ms);
        void Pause();
        void Resume();
    protected:
    private:
        //Mix_Music* m_bgmMus;
        atomic<Mix_Chunk*> m_bgmMusHead,m_bgmMusLoop;

        int m_loops;
};

extern BgmMgr bgm;

#endif // BGMMGR_H
