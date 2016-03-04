#ifndef TITLE_H
#define TITLE_H

#include "Snow.h"
#include <vector>

class Title : public Snow::Activity
{
    public:
        Title();
        void OnInit();
        void OnShow();
        void OnNext();
        void OnDraw();
        void OnHide();
    protected:
    private:

        /* ������İ�ť�ܹ� */
        enum ButtonWork{
            START,
            STARTEXTRA,
            PRATCICE,
            RANK,
            GAMEPADKEYSET,
            MUSICROOM,
            EXIT
        };
        struct Button{
            SDL_Texture* tex;
            Uint8 alpha;
            SDL_Rect dst_draw;  //����ʱ��λ��
            SDL_Point dst;   //ʵ��λ��
            ButtonWork btn;
        };
        enum{
            SHOWING,NORMAL,HIDING
        }m_buttonState;
        std::vector<Button> m_buttons;
        int m_actButton,m_btn_y;
        Snow::Timer m_btn_tmr;
        void clearButton();
        void addButton(const char* file,ButtonWork);
        void doButton(ButtonWork);
        void showButton();
        void hideButton();
        void onNextButton();
        void onDrawButton();

        /* ���¼�ѡ�� */
        void selectUp();
        void selectDown();
        void selectEnter();
};

#endif // TITLE_H
