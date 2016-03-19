#ifndef BGMMGR_H
#define BGMMGR_H
#include <string>
#include "Snow.h"
#include <atomic>

struct Bgm{
    Mix_Chunk* head,*loop;
    Bgm(const std::string& rv);
    ~Bgm();
};

class BgmMgr
{
    private:
        Bgm* m_bgm;
    public:
        void LoadMusic(const std::string& rv);
        void UseMusic(Bgm*);    //auto destroy bgm struct.
        Mix_Chunk* GetBGMLoop(){return m_bgm->loop;}
        Bgm* GetBGMObj(){return m_bgm;}
        void Play();
        void Clear();
        void Stop(int ms);
        void Pause();
        void Resume();
    protected:
};

extern BgmMgr bgm;

#endif // BGMMGR_H
