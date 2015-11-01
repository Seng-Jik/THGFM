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
        //��Һţ������ţ�����״̬��trueΪ���£�����Ϊ������
        void LoadConfig();

        static void RefreshGamePadList();
        static int GetLinkedGamePadCount();
    protected:
    private:
        static Key kb2k(SDL_Keycode);  //���̰���ת��Ϸ����
        static int gk2p(SDL_JoystickID);    //���ֱ�������ȡ��Һ�(1/2)������0Ϊ�޷���ȡ��Һ�
        static Key m_k [2] [32];
        static int m_devs [2];
        static SDL_Joystick* m_gamepads [2];
        static SDL_JoystickID m_gid [2];
};
