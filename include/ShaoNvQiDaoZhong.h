#ifndef SHAONVQIDAOZHONG_H
#define SHAONVQIDAOZHONG_H
#include "Snow.h"

class ShaoNvQiDaoZhong:Snow::Activity
{
    public:
        ShaoNvQiDaoZhong(void((*func)(Snow::THREAD_ID)),Snow::Activity*);
        static void Init();
        void OnShow();
        void OnHide();
        void OnDraw();
        void OnNext();
        ~ShaoNvQiDaoZhong();
    protected:
    private:
        static SDL_Texture* m_ani[89];    /* 0~2保留，3~29为启动动画，30~59为循环动画，60~88为结束动画。 */
        static const int BEG_BEG = 3,BEGIN_END = 29,LOOP_BEG = 30,LOOP_END = 59,END_BEG = 60,END_END = 88;
        const SDL_Rect  DSTRECT = {896,624,384,96};
        enum{
            LOOP,
            END
        }state;
        int m_frame;
};

#endif // SHAONVQIDAOZHONG_H
