<<<<<<< HEAD
#ifndef CHARDATA_H
#define CHARDATA_H
#include "Snow.h"
#include "Player.h"
class CharData
{
    public:
        virtual bool BoomOnNext(Player*)=0;
        virtual void BoomOnDraw()=0;
        virtual void BoomReset(Player*)=0;
    protected:
    private:
};

class Marisa:public CharData{
    private:
        SDL_Texture* m_marisa_boom;
        Snow::FrameTimer m_tmr;
        SDL_Rect m_r;
        float m_mask;
    public:
        void Init();
        virtual bool BoomOnNext(Player*);
        virtual void BoomOnDraw();
        virtual void BoomReset(Player*);
};
extern Marisa marisa;

class Reimu:public CharData{
    private:
        SDL_Texture* m_reimu_w;
        SDL_Texture* m_reimu_h;
        Snow::FrameTimer m_tmr;
        SDL_Rect m_wr[2];
        SDL_Rect m_hr[2];
        float m_mask;
    public:
        void Init();
        virtual bool BoomOnNext(Player*);
        virtual void BoomOnDraw();
        virtual void BoomReset(Player*);
};
extern Reimu reimu;

#endif // CHARDATA_H
=======
#ifndef CHARDATA_H
#define CHARDATA_H
#include "Snow.h"
#include "Player.h"
class CharData
{
    public:
        virtual bool BoomOnNext(Player*)=0;
        virtual void BoomOnDraw()=0;
        virtual void BoomReset(Player*)=0;
    protected:
    private:
};

class Marisa:public CharData{
    private:
        SDL_Texture* m_marisa_boom;
        Snow::FrameTimer m_tmr;
        SDL_Rect m_r;
        float m_mask;
    public:
        void Init();
        virtual bool BoomOnNext(Player*);
        virtual void BoomOnDraw();
        virtual void BoomReset(Player*);
};
extern Marisa marisa;

class Reimu:public CharData{
    private:
        SDL_Texture* m_reimu_w;
        SDL_Texture* m_reimu_h;
        Snow::FrameTimer m_tmr;
        SDL_Rect m_wr[2];
        SDL_Rect m_hr[2];
        float m_mask;
    public:
        void Init();
        virtual bool BoomOnNext(Player*);
        virtual void BoomOnDraw();
        virtual void BoomReset(Player*);
};
extern Reimu reimu;

#endif // CHARDATA_H
>>>>>>> 720c0b59a97be74f02a7c85da646f3f62bece3a5
