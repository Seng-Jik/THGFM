#ifndef SHAONVQIDAOZHONG_H
#define SHAONVQIDAOZHONG_H
#include "Snow.h"

class ShaoNvQiDaoZhong:Snow::Activity
{
    public:
        ShaoNvQiDaoZhong(void((*func)(Snow::THREAD_ID)),Snow::Activity*);   //一次性活动，用完自动delete
        //第一个为显示少女祈祷中时的那个多线程函数，第二个为完成后跳转到的活动，如果使用Gosub则传入nullptr。
        static void Init();
        void SendMsg(int);  //向线程发送数据
        void OnHide();
        void OnDraw();
        void OnNext();
    protected:
    private:
        static SDL_Texture* m_ani;
        static const int BEG_BEG = 3,BEGIN_END = 29,LOOP_BEG = 30,LOOP_END = 59,END_BEG = 60,END_END = 88;
        const SDL_Rect  DSTRECT = {896,624,384,96};
        int m_frame;
        Snow::Thread m_thd;
        Snow::Activity* m_act;
};

#endif // SHAONVQIDAOZHONG_H
