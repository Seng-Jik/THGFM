#ifndef GAMEUI_H
#define GAMEUI_H
#include "Snow.h"
#include <string>

class GameUI
{
    private:
        //Player State Show And Game Manage Logic
        SDL_Texture* m_heart;
        SDL_Texture* m_bomb;
        int m_life;
        int m_bombNum;
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

        SDL_Texture* m_spellCardStar;
        float m_scHP,m_showSCHP;   //实际HP和用于平滑显示的HP
    public:
        void Init();
        void OnDraw();
        void OnNext();

        //Player
        void SetLifeAndBomb(int life,int bomb){m_life = life;m_bombNum = bomb;}
        void OnGetLife();  //玩家取得生命碎片时
        void OnPlayerBeKilled();    //玩家被杀死时
        void OnGetBomb();  //玩家取得炸弹碎片时
        void OnUseBomb();   //玩家使用炸弹时
        inline bool CanUseBomb(){  //玩家是否可使用炸弹
            return m_bombNum > 0;
        }
        inline int GetLife(){return m_life;}

        //Boss
        void OpenBoss();    //出现Boss时
        void CloseBoss();   //Boss战结束时
        inline void ShowHPLine(){m_bossHPLineShow = true;};
        inline void HideHPLine(){m_bossHPLineShow = false;};
        void SetSpellCard(const std::string& scName);   //显示符卡名称时
        inline void UpdateSCHP(double hp){  //更新符卡剩余生命值
            m_scHP = hp;
        };
        inline void ResetHPLine(){m_showSCHP = 0;}
    protected:
};

extern GameUI gameUI;

#endif // GAMEUI_H
