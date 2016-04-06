#include "KeyMapAct.h"
#include "Player.h"
#include "StageMgr.h"
class Boss;
class WSTGame : public KeyMapAct
{
    public:
        WSTGame();
        void OnInit();
        void OnShow();
        void OnDraw();
        void OnNext();
        void OnEvent(int p,Key,bool);
        void Pause();
        void OnResume();

        void StageClear();

        void GrpShake(int frame);
        void GrpColorShake(int ms);
        void FadeInBossSpellCardBgp();
        ~WSTGame();
    protected:
    private:
        bool m_dbg_doubleSpeed = false;

        SDL_Texture* m_stageClearScreen;
        SDL_Rect m_stageClearScreenRect;
        int m_stageClearEffCnt = 0;    //0为关闭动画,大于0为动画中

        SDL_Texture* m_pGameGraphic;
        SDL_Texture* m_pBossBgp;
        Uint8 m_bossBgpAlpha;
        enum{SHOWING,NORMAL}m_bossBgpState;

        int m_shkTmr = 0;
        Uint8 m_black_start_alpha;
};

extern WSTGame* wstg;
