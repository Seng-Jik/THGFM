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
        static SDL_Texture* m_ani;
        static const int BEG_BEG = 3,BEGIN_END = 29,LOOP_BEG = 30,LOOP_END = 59,END_BEG = 60,END_END = 88;
        const SDL_Rect  DSTRECT = {896,624,384,58};
        int m_frame;
        std::thread* m_thd;
        Snow::Activity* m_act;
};

#endif // SHAONVQIDAOZHONG_H
