#ifndef TOUHOUGAMELOGO_H
#define TOUHOUGAMELOGO_H

#include <Snow.h>


class TouHouGameLogo : public Snow::Activity
{
    public:
        TouHouGameLogo();
        void OnShow();
        void OnNext();
        void OnDraw();
        void OnHide();
        virtual ~TouHouGameLogo();
    protected:
    private:
        Snow::FrameTimer m_tmr;
        Snow::Sprite m_center;
        Snow::Sprite m_rightBottom;
        Snow::Sprite m_rightTop;
};
extern TouHouGameLogo* touHouGameLogo;
#endif // TOUHOUGAMELOGO_H
