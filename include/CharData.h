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
        virtual void BulletInstaller(double power,int cnt,double x,double y) = 0;
        virtual void InstallDeatAnimation(int x,int y) = 0;
        virtual std::string CharName()=0;
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
        virtual void BulletInstaller(double power,int cnt,double x,double y);
        virtual void InstallDeatAnimation(int x,int y);
        virtual std::string CharName(){return "MARISA";}
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
        float m_spd;
    public:
        void Init();
        virtual bool BoomOnNext(Player*);
        virtual void BoomOnDraw();
        virtual void BoomReset(Player*);
        virtual void BulletInstaller(double power,int cnt,double x,double y);
        virtual void InstallDeatAnimation(int x,int y);
        virtual std::string CharName(){return "REIMU";}
};
extern Reimu reimu;

#endif // CHARDATA_H
