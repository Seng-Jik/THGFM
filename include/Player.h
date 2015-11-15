#include "Snow.h"
#include "KeyMapAct.h"
class Player
{
    private:
        static SDL_Texture* m_images [2] [5];
        enum {LIVING,DEAD,BIRTHING}m_live;
        int m_birthTimer;

        int m_invin = 0;
        double m_x = 0,m_y = 0;
        SDL_Rect m_r = {0,0,80,95};
        int m_cnt = 0;
        bool m_k [5] = {false};
        int m_playerNum = 0;
        int m_playerImageNum = 0;


    public:
        static void Init();

        Player();
        void SetPlayerImage(int);   //0-Reimu,1-Marisa
        void OnDraw();
        void OnNext();
        void OnEvent(Key,bool);
        void GetPos(double& x,double& y){x=m_x;y=m_y;}
        void Invincible(int);
        void SetPlayer(int);
        inline double GetX(){return m_x;}
        inline double GetY(){return m_y;}
        void Kill();
        void Birth();
        virtual ~Player();
    protected:
};

extern Player player [1];
