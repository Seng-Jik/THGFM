<<<<<<< HEAD
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

        void GrpShake(int frame);
        void GrpColorShake(int ms);
        ~WSTGame();
    protected:
    private:
        SDL_Texture* m_pGameGraphic;
        int m_shkTmr = 0;
};

extern WSTGame* wstg;
=======
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

        void GrpShake(int frame);
        void GrpColorShake(int ms);
        ~WSTGame();
    protected:
    private:
        SDL_Texture* m_pGameGraphic;
        int m_shkTmr = 0;
};

extern WSTGame* wstg;
>>>>>>> 720c0b59a97be74f02a7c85da646f3f62bece3a5
