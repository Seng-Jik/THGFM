#ifndef EFFECTMGR_H
#define EFFECTMGR_H
#include <list>
#include "Snow.h"
class EffectMgr //用来管理和游戏内容完全无关的小特效
{
    public:
        EffectMgr();
        static void Init();
        void Install(int style,int x,int y);
        void OnDraw();
        void OnNext();
        void Reset();
        void Kill(int n);
        ~EffectMgr();
    protected:
    private:
        static struct EffectStyle{
            SDL_Texture** tex;
            int texCount;
            int frameWait;
        }m_effStyles[1];
        struct Eff{
            int style = -1,cnt;
            int x,y;
        }m_effs[1024];
        int m_searchTop = 0;
};

extern EffectMgr effMgr;

#endif // EFFECTMGR_H
