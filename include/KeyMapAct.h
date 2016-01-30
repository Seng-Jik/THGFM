#pragma once
#include "Snow/Activity.h"

enum Key{
    T_UP,T_DOWN,
    T_RIGHT,T_LEFT,
    T_SHOT,T_BOOM,T_SLOW,
    T_PAUSE,
    T_ENTER,
    T_ESC,
    T_NONE
};

class KeyMapAct : public Snow::Activity
{
    public:
        KeyMapAct();
        static void Init();
        void OnEvent(const SDL_Event& e);
        virtual void OnEvent(int,Key,bool) = 0;
        //玩家号，按键号，按键状态（true为按下，否则为弹出）
        static void LoadConfig();
    protected:
    private:
        static Key kb2k(SDL_Keycode);  //键盘按键转游戏按键
        static Key m_k [2] [32];
        static int m_axis[2] [2];
};
