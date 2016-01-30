#ifndef STAGETITLE_H
#define STAGETITLE_H
#include "Snow.h"

class StageTitle
{
    private:
        int m_begTime = 0;
        int m_endTime = 0;
        int m_timeLen;
        int m_cnt;
        SDL_Texture* m_tex = nullptr;
        int m_w,m_h;
        SDL_Rect r;
    public:
        void Set(int beg,int end,const std::string& image);
        void OnNext();
        void OnDraw();
        inline int GetBeginTime(){
            return m_begTime;
        }
        inline int GetEndTime(){
            return m_endTime;
        }
    protected:
};

extern StageTitle stageTitle;

#endif // STAGETITLE_H
