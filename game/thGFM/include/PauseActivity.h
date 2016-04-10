#ifndef PAUSEACTIVITY_H
#define PAUSEACTIVITY_H

#include "KeyMapAct.h"
#include "Snow.h"

class PauseActivity : public KeyMapAct
{
    public:
        void ResumeGame();
        void OnShow();
        void OnHide();
        void OnDraw();
        void OnNext();
        void OnEvent(int p, Key k, bool b);
    protected:
    private:
        Snow::Surface m_bg;
        Snow::Sprite m_bgt;
        Snow::Sprite m_bgt_o;
        Snow::Timer m_tmr;
        enum{PAUSEING,RESUMEING,MENU}m_state;
};

extern PauseActivity* pause;

#endif // PAUSEACTIVITY_H
