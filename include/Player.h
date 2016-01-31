<<<<<<< HEAD
#pragma once
#include "Snow.h"
#include "KeyMapAct.h"
class CharData;
class Player
{
    private:
        static SDL_Texture* m_images [2] [5];
        enum {LIVING,DEATHVS,DEAD,BIRTHING}m_live;
        int m_birthTimer;

        int m_invin = 0;
        int m_deathVS = 0;  //¾öËÀ£¡
        double m_x = 0,m_y = 0;
        SDL_Rect m_r = {0,0,80,95};
        int m_cnt = 0;
        bool m_k [8] = {false};
        int m_playerNum = 0;
        int m_playerImageNum = 0;
        int m_playerBulletStyle = 0;
        int m_powerMode = 0;
        double m_powerValue = 0;

        bool m_booming = false;
        CharData* m_charData;


    public:
        static void Init();

        Player();
        inline bool GetShotPressed(){return m_k[5];}
        void SetPlayerImage(int);   //0-Reimu,1-Marisa
        void OnDraw();
        void OnNext();
        void OnEvent(Key,bool);
        void GetPos(double& x,double& y){x=m_x;y=m_y;}
        void Invincible(int);
        void SetPlayer(int);
        inline double GetX(){return m_x;}
        inline double GetY(){return m_y;}
        inline bool Living(){return m_live == LIVING || m_live == BIRTHING;}
        void Kill();
        void Birth();
        void AddPower(double power);
        void ClearKey();
        inline bool Booming(){return m_booming;}
        virtual ~Player();
    protected:
};

extern Player player [1];
=======
#pragma once
#include "Snow.h"
#include "KeyMapAct.h"
class CharData;
class Player
{
    private:
        static SDL_Texture* m_images [2] [5];
        enum {LIVING,DEATHVS,DEAD,BIRTHING}m_live;
        int m_birthTimer;

        int m_invin = 0;
        int m_deathVS = 0;  //¾öËÀ£¡
        double m_x = 0,m_y = 0;
        SDL_Rect m_r = {0,0,80,95};
        int m_cnt = 0;
        bool m_k [8] = {false};
        int m_playerNum = 0;
        int m_playerImageNum = 0;
        int m_playerBulletStyle = 0;
        int m_powerMode = 0;
        double m_powerValue = 0;

        bool m_booming = false;
        CharData* m_charData;


    public:
        static void Init();

        Player();
        inline bool GetShotPressed(){return m_k[5];}
        void SetPlayerImage(int);   //0-Reimu,1-Marisa
        void OnDraw();
        void OnNext();
        void OnEvent(Key,bool);
        void GetPos(double& x,double& y){x=m_x;y=m_y;}
        void Invincible(int);
        void SetPlayer(int);
        inline double GetX(){return m_x;}
        inline double GetY(){return m_y;}
        inline bool Living(){return m_live == LIVING || m_live == BIRTHING;}
        void Kill();
        void Birth();
        void AddPower(double power);
        void ClearKey();
        inline bool Booming(){return m_booming;}
        virtual ~Player();
    protected:
};

extern Player player [1];
>>>>>>> 720c0b59a97be74f02a7c85da646f3f62bece3a5
