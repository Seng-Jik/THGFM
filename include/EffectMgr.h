#ifndef EFFECTMGR_H
#define EFFECTMGR_H
#include <list>
#include "Snow.h"
class EffectMgr
{
    public:
        EffectMgr();
        static void Init();
        void Install(int num,int x,int y);
        void OnDraw();
        void OnNext();
        void Reset();
        ~EffectMgr();
    protected:
    private:
        struct EffectStyle{
            SDL_Texture** tex;
            int texCount;
            int frameWait;
        }m_effStyles[1];
        struct Eff{
            int style,cnt;
            int x,y;
        };
        std::list<Eff> m_effs;
};

#endif // EFFECTMGR_H
