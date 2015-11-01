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
        void OnEvent(const SDL_Event& e);
        virtual void OnEvent(int,Key,bool) = 0;
        //玩家号，按键号，按键状态（true为按下，否则为弹出）
        void LoadConfig();

        static void RefreshGamePadList();
        static int GetLinkedGamePadCount();
    protected:
    private:
        static Key kb2k(SDL_Keycode);  //键盘按键转游戏按键
        static int gk2p(SDL_JoystickID);    //从手柄按键获取玩家号(1/2)，返回0为无法获取玩家号
        static Key m_k [2] [32];
        static int m_devs [2];
        static SDL_Joystick* m_gamepads [2];
        static SDL_JoystickID m_gid [2];
};
