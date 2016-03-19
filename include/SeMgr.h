#ifndef SEMGR_H
#define SEMGR_H
#include "Snow.h"
enum SeName{
    BONUS,
    BONUS2,
    CANCEL00,
    CARDGET,
    CAT00,
    CAT01,
    CH00,
    CH01,
    DAMAGE00,
    DAMAGE01,
    ENEP00,
    ENEP01,
    ENEP02,
    EXTEND,
    GRAZE,
    GUN00,
    HINT00,
    INVALID,
    ITEM00,
    KIRA00,
    KIRA01,
    KIRA02,
    LAZER00,
    LAZER01,
    MSL,
    NEP00,
    OK00,
    OPTION,
    PAUSE,
    PLDEAD00,
    PLST00,
    POWER0,
    POWER1,
    POWERUP,
    SELECT00,
    SLASH,
    TAN00,
    TAN01,
    TAN02,
    TIMEOUT,
    TIMEOUT2,
    WARPL,
    WARPR,
    WATER
};
class SeMgr
{
    public:
        static void Init();
        void Play(SeName);
        void Pause();
        void Resume();
    protected:
    private:
        static void loadSe(SeName,const std::string& se);
        static Mix_Chunk* m_se[50];
};
extern SeMgr se;

#endif // SEMGR_H
