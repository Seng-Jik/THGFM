#ifndef SHAONVQIDAOZHONG_H
#define SHAONVQIDAOZHONG_H
#include "Snow.h"
#include <thread>
#include <atomic>
class ShaoNvQiDaoZhong:public Snow::Activity
{
    public:
        ShaoNvQiDaoZhong(std::thread*,Snow::Activity*);   //一次性活动，用完自动delete
        //第一个为显示少女祈祷中时的那个多线程函数，第二个为完成后跳转到的活动，如果使用Gosub则传入nullptr。
        static void Init();
        static void EndShaoNv();    //停止少女祈祷中画面
        void OnShow();
        void OnHide();
        void OnDraw();
        void OnNext();
    protected:
    private:
        static std::atomic_bool m_continueRun;
        static SDL_Texture* m_ani1,*m_ani2;

        double m_yinyang_angle;
        Uint8 m_yinyang_alpha,m_qidao_alpha;
        bool m_qidao_alpha_down;

        enum{
            FADEIN,RUNNING,FADEOUT
        }m_state;

        std::thread* m_thd;
        Snow::Activity* m_act;
};

#endif // SHAONVQIDAOZHONG_H
