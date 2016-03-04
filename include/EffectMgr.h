#ifndef EFFECTMGR_H
#define EFFECTMGR_H
#include <list>
#include "Snow.h"
class EffectMgr //用来管理和游戏内容完全无关的小特效
{
    public:
        EffectMgr();
        static void Init();
        void InstallFrameAnimation(int style,int x,int y);
        void InstallZoomOutAnimation(SDL_Texture* image,const SDL_Rect& r,bool alphaDown,const SDL_Point& poi,const double angle);
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
        }m_effStyles[32];
        struct Eff{ //Frame Animations
            int style = -1,cnt;
            int x,y;
        }m_effs[1024];
        int m_searchTop = 0;

        struct ZoomOutEff{
            SDL_Texture* tex = nullptr;
            SDL_Rect rect;
            SDL_Point centerPoi;
            double angle;
            Uint8 alpha;
            bool alphaDown;
        }m_zoomOutEffs[1024];
        int m_zoomOutEffSearchTop=0;
};

extern EffectMgr effMgr;

#endif // EFFECTMGR_H
