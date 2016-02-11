#ifndef BASEBOSS_H
#define BASEBOSS_H

#include <string>
#include "Snow.h"
//#include "MathFunc.h"
#include <queue>
#include "BossConversation.h"
class Boss
{
    friend int SCCreateBlt(Boss* s,double x,double y,double angle,double spd,int minLiveTime,int style);
    friend class BulletMgr;
    private:
        int* m_mainCnt;  //全局帧计数器
        int m_cnt;  //独立帧计数器
        bool m_bouns;   //是否应当奖励
        bool m_invi;    //Boss是否无敌
        BossConversation* m_bossConversation;   //Boss对话活动
        float m_hpAttackMul = 1;    //攻击倍率

        bool m_live = true;    //存活标志
        bool m_collEnable;
        bool m_midway;
        std::string m_conversation;
        std::string m_basePath;

        SDL_Texture* m_images[10];
        int m_images_w,m_images_h;

        int m_imageUsing;
        bool m_firsShow;
        double m_x,m_y,m_spd,m_aspd,m_angle;
        int m_cnt_begin;  //开始时的帧数
        double m_fullHP;    //备份的当前符卡满HP

        int m_spellCardNum; //符卡总数
        struct SpellCard{
            int endTime;    //符卡死亡终帧
            bool isSpellCard;   //是否为符卡
            double hp;    //符卡生命值
            int scPartten;  //符卡工作模式
            int bgPartten;  //背景图
            std::string title;  //符卡名称
        };
        std::queue<SpellCard> m_spellCards;
        std::vector<int> m_bullets;
    public:
        ~Boss();
        void LoadRV(const std::string&,const std::string& basePath,int* cnt);
        void OnBirth();
        void OnDraw();
        void OnSCBGDraw();
        void OnNext();
        void OnConersationFinished();
        inline bool Live(){return m_live;};
        inline void KillHP(double hp){m_spellCards.front().hp-=m_hpAttackMul*hp;}
        inline bool IsInvi(){return m_invi;}
        inline int GetSpellCardNum(){return m_spellCardNum;}    //取符卡剩余数
        int birthTime;

    protected:
};

#endif // BASEBOSS_H
