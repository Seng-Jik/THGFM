#include "KeyMapAct.h"
#include "Player.h"
#include "StageMgr.h"

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
        ~WSTGame();
    protected:
    private:
        bool m_dbg_doubleSpeed = false;

        SDL_Texture* m_stageClearScreen;
        SDL_Rect m_stageClearScreenRect;
        int m_stageClearEffCnt = 0;    //0Ϊ�رն���,����0Ϊ������

        SDL_Texture* m_pGameGraphic;
        int m_shkTmr = 0;
};

extern WSTGame* wstg;
