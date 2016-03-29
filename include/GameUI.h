#ifndef GAMEUI_H
#define GAMEUI_H
#include "Snow.h"
#include "MathFunc.h"
#include <string>
#include "Tools.h"

//SC means SpellCard, A means alpha
const Uint8 SC_A = 128;
const int SC_TIME = 30;
extern int sc_x;
const int SC_Y = HEIGHT/2-16;
const int END_X = WIDTH - 128;
const int END_Y = 56;

class GameUI
{
    private:
        //Player State Show And Game Manage Logic
        SDL_Texture* m_heart;
        SDL_Texture* m_bomb;
        int m_life;
        int m_bombNum;
        int m_scNameTime;
        int m_rebirth = 0;

        //Boss State Show
        bool m_bossOpened;
        bool m_showSC;
        SDL_Texture* m_bossHPLine,*m_bossHPLineShell;
        float m_bossHPLineAlpha;
        bool m_bossHPLineShow;
        //Spell Card Name
        Snow::Sprite m_scName;
        Snow::FrameTimer m_scName_tmr;
        Snow::Font m_font;
        bool m_scName_animation;
        bool m_scName_show;
        bool m_scName_downAnimation;

        SDL_Texture* m_spellCardStar;
        float m_scHP,m_showSCHP;   //ʵ��HP������ƽ����ʾ��HP

        void doScDownAnimation();   //ִ�з������½�����
    public:
        void Init();
        void OnDraw();
        void OnNext();

        //Player
        void SetLifeAndBomb(int life,int bomb){m_life = life;m_bombNum = bomb;}
        void OnGetLife();  //���ȡ��������Ƭʱ
        void OnPlayerBeKilled();    //��ұ�ɱ��ʱ
        void OnGetBomb();  //���ȡ��ը����Ƭʱ
        void OnUseBomb();   //���ʹ��ը��ʱ
        inline bool CanUseBomb(){  //����Ƿ��ʹ��ը��
            return m_bombNum > 0;
        }
        inline int GetLife(){return m_life;}

        //Boss
        void OpenBoss();    //����Bossʱ
        void CloseBoss();   //Bossս����ʱ
        inline void ShowHPLine(){m_bossHPLineShow = true;};
        inline void HideHPLine(){m_bossHPLineShow = false;};
        void SetSpellCard(const std::string& scName);   //��ʾ��������ʱ
        inline void KillSpellCard(){
            m_scName_show = false;
        }
        inline void UpdateSCHP(float hp){  //���·���ʣ������ֵ
            m_scHP = hp;
        };
        inline void ResetHPLine(){m_showSCHP = 0;}
    protected:
};

extern GameUI gameUI;

#endif // GAMEUI_H
