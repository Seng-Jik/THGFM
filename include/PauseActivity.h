#ifndef PAUSEACTIVITY_H
#define PAUSEACTIVITY_H

#include "KeyMapAct.h"
#include "Snow.h"
#include "MathFunc.h"
#include "SeMgr.h"

const int ALLCHOICE = 6;
const int CHOICEX = 100;
const int CHOICEY = 350;
const int MENUSHOWTIME = 48;    //frames
const int MENUHIDETIME = 48;    //frames
const int MENUWAITTIME = 22;
const int MENUCHANGETIME = 15;

enum BtnFunc{    RESUME = 0,    RESTART,    RETRY,    SETTINGS,    TOTITLE,    EXIT };
extern SeMgr se;

struct btn{
    BtnFunc func;
    bool enabled;
    Snow::Sprite sp;
};

class PauseActivity : public KeyMapAct
{
public:
    PauseActivity();
    ~PauseActivity();
    void AddBtns(BtnFunc i_btns[], int i_len = 0);
    void ResumeGame();
    void OnShow();
    void OnDraw();
    void OnNext();
    void OnEvent(int p, Key k, bool b);

private:
    //bg
    Snow::Surface m_bg;
    Snow::Sprite m_bgt;
    Snow::Sprite m_bgt_o;
    Snow::Sprite m_ptr;
    Snow::Timer m_tmr;
    bool m_change_quick_finish;
    enum { SHOWING, WAITING, CHOOSING, FINISHED, HIDING } m_state, m_sub_state;

    btn m_btns[ALLCHOICE];  //and the position will be automatically calc-ed according to the near btn.
    int m_show_time, m_hide_time, m_wait_time, m_change_time, m_cycle;
    int m_wait_frame_adddec;
    int m_ptr_state;
    inline int SetCycle(int i_cycle){ if(i_cycle > 3) return i_cycle - 4; return i_cycle;}

    //submenu
    Snow::Sprite m_submenu_ask_sp[2];
    Snow::Sprite m_submenu_ptr_sp;
    int m_submenu_ptr;
    int m_submenu_state;
    bool m_submenu;

    //score modules
    Snow::Sprite m_score;
    int m_num_color;
    SDL_Rect m_score_rect;
    int m_score_rect_x;
    int m_score_num;
    int m_score_pt[11];
    SDL_Texture * m_score_tex[10]; //0-9
};

extern PauseActivity* pause;

#endif // PAUSEACTIVITY_H
