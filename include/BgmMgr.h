#ifndef BGMMGR_H
#define BGMMGR_H
#include <string>
#include "Snow.h"
class BgmMgr
{
    public:
        void LoadMusic(const std::string&,int loops);
        void Play();
        void Clear();
        void Stop(int ms);
        void Pause();
        void Resume();
    protected:
    private:
        Mix_Music* m_bgmMus;
        int m_loops;
};

extern BgmMgr bgm;

#endif // BGMMGR_H
