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
    void Reset();
    void Init();
    void Show();
    void Hide();
    void SetTime(int i_frame);
    void SetAlpha(float i_a);
    void OnNext();
    void OnDraw();

private:
    //the picture of color is the last
    SDL_Texture* m_tex[11];
    SDL_Rect m_rect[5];
    int m_sec_0, m_sec_1, m_msec_0, m_msec_1, m_animetime;
    float m_alpha;
    bool m_show, m_x, m_y, m_move, m_init;
    const int COLON = 10;
};

extern ScClock scClock;

#endif // SCCLOCK_H
