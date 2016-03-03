#ifndef TITLE_H
#define TITLE_H

#include "Snow.h"


class Title : public Snow::Activity
{
    public:
        void OnShow();
        void OnNext();
        void OnDraw();
        void OnHide();
    protected:
    private:
};

#endif // TITLE_H
