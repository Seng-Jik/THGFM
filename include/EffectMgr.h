#ifndef EFFECTMGR_H
#define EFFECTMGR_H
#include <list>
#include "Snow.h"
class EffectMgr //�����������Ϸ������ȫ�޹ص�С��Ч
{
    public:
        EffectMgr();
        static void Init();
        void Install(int style,int x,int y);
        void OnDraw();
        void OnNext();
        void Reset();
        ~EffectMgr();
    protected:
    private:
        static struct EffectStyle{
            SDL_Texture** tex;
            int texCount;
            int frameWait;
        }m_effStyles[1];
        struct Eff{
            int style,cnt;
            int x,y;
        }m_effs[32];
};

extern EffectMgr effMgr;

#endif // EFFECTMGR_H
