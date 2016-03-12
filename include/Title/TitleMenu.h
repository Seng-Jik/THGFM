#ifndef TitleMenu_H
#define TitleMenu_H

#include "../Snow.h"
#include "../KeyMapAct.h"
#include <vector>

class TitleMenu : public KeyMapAct
{
    public:
        TitleMenu();
        void OnInit();
        void OnShow();
        void OnNext();
        void OnDraw();
        void OnHide();
        void OnEvent(int,Key,bool);
    protected:
    private:
        SDL_Texture* m_bgp;

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
            SHOWING,NORMAL,HIDING,SELECTER_MOVING,SELECTER_FLASHING,HIDDEN
        }m_buttonState;
        std::vector<Button> m_buttons;
        Uint16 m_actButton,m_btn_y;
        Snow::Timer m_btn_tmr;
        Snow::Sprite m_selecter;
        Uint8 m_selecterAlpha;
        bool m_selecter_alpha_downing;
        int m_selecter_trg,m_selecter_old;  //Value Y
        void clearButton();
        void addButton(const char* file,ButtonWork);
        void doButton(ButtonWork);
        void setActButton(int);
        void showButton();
        void hideButton();
        void onNextButton();
        void onDrawButton();

        /* ���¼�ѡ�� */
        void selectUp();
        void selectDown();
        void selectEnter();
};

extern TitleMenu titleMenu;

#endif // TitleMenu_H
