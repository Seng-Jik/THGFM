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
        Snow::Sprite m_select[4];
        Snow::Sprite m_ptr;
        int m_ptr_state;
        //last = all - 1
        const int ALLCHOICE = 4;
        const int CHOICEX = 500;
        const int CHOICEY = 250;
        //Resume, Settings, Return_to_Title, Quit_the_Game
        Snow::Sprite m_bgt;
        Snow::Sprite m_bgt_o;
        Snow::Timer m_tmr;
        enum{PAUSEING,RESUMEING,MENU}m_state;
};

extern PauseActivity* pause;

#endif // PAUSEACTIVITY_H
