#ifndef BACKGROUND_H
#define BACKGROUND_H


class Background
{
    public:
        virtual void OnDraw() = 0;
        virtual void  OnNext() = 0;
    protected:
    private:
};

#endif // BACKGROUND_H
