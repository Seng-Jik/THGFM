/*
the clocker system of spellcard
written by MaikazeSekai
*/
#ifndef SCCLOCK_H
#define SCCLOCK_H
#include "Snow.h"
#include "MathFunc.h"
#include "Tools.h"
#include "Snow/Debug.h"
#include <string>

class ScClock{
public:
    void Init();
    void Show();
    void Hide();
    void SetTime(int i_frame);
    void OnNext();
    void OnDraw();

private:
    //the picture of color is the last
    void Reset();
    void SetAlpha(Uint8 alpha);
    SDL_Texture* m_tex[2][11];
    SDL_Rect m_rect[5];
    int m_sec_0, m_sec_1, m_msec_0, m_msec_1;

    bool m_visible = false;

    //Animation
    enum{
        SHOWING_S1, //½×¶Î1
        SHOWING_S2, //½×¶Î2
        HIDING,
        NONE
    }m_anime = NONE;
    Snow::FrameTimer m_animeTmr;

public:
    inline bool GetVisible(){return m_visible;}
};

extern ScClock scClock;

#endif // SCCLOCK_H
