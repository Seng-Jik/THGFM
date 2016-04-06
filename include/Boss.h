#ifndef BASEBOSS_H
#define BASEBOSS_H

#include <string>
#include <deque>
#include "Snow.h"
//#include "MathFunc.h"
#include <queue>
#include <set>
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
        BossConversation* m_bossConversation_whenKilled;   //Boss死亡后的对话活动

        bool m_live = true;    //存活标志
        bool m_collEnable;
        bool m_midway;
        std::string m_conversation;
        std::string m_conversation_whenKilled;
        std::string m_basePath;

        SDL_Texture* m_images[10] = {nullptr};
        int m_images_w,m_images_h;

        int m_imageUsing;
        bool m_firsShow;
        double m_x,m_y,m_spd,m_aspd,m_angle;
        int m_cnt_begin;  //开始时的帧数
        double m_fullHP;    //备份的当前符卡满HP

        int m_spellCardNum; //符卡总数
        struct SpellCard{
            int timeLimit;    //符卡时长限制
            bool isSpellCard;   //是否为符卡
            double hp;    //符卡生命值
            int scPartten;  //符卡工作模式
            int bgPartten;  //背景图
            std::string title;  //符卡名称
            Snow::Bundle<256>* scBgData; //符卡背景数据
        };
        std::queue<SpellCard> m_spellCards;
        std::vector<int> m_bullets;
        Snow::Bundle<256> m_scParttenData;

        //结束时间
        int m_endTime;  //应当到达的时间
    public:
        ~Boss();
        inline int GetCnt(){return m_cnt;}
        void LoadRV(const std::string&,const std::string& basePath,int* cnt,const std::string& playerChar);
        void OnBirth();
        void OnDraw();
        void OnSCBGDraw();
        void OnNext();
        void OnConersationFinished();
        inline bool Live(){return m_live;};
        inline void KillHP(double hp){m_spellCards.front().hp-=hp;}
        inline bool IsInvi(){return m_invi;}
        inline int GetSpellCardNum(){return m_spellCardNum;}    //取符卡剩余数
        inline void ResetCnt(){m_cnt = 0;}
        inline int GetSCnt(){return *m_mainCnt - m_cnt_begin;}
        inline bool HaveSpellCardBgp(){return m_spellCards.front().bgPartten != -1;}
        int birthTime;
        inline void GetPos(double& x,double& y){x=m_x,y=m_y;}

    protected:
};

#endif // BASEBOSS_H
