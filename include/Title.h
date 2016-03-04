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

        /* 最基本的按钮架构 */
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
            SDL_Rect dst_draw;  //绘制时的位置
            SDL_Point dst;   //实际位置
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

        /* 上下键选择 */
        void selectUp();
        void selectDown();
        void selectEnter();
};

#endif // TITLE_H
