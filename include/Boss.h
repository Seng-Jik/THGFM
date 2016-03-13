#ifndef BASEBOSS_H
#define BASEBOSS_H

#include <string>
#include <deque>
#include "Snow.h"
//#include "MathFunc.h"
#include <queue>
#include "BossConversation.h"
class Boss
{
    friend int SCCreateBlt(Boss* s,double x,double y,double angle,double spd,int minLiveTime,int style);
    friend class BulletMgr;
    private:
        int* m_mainCnt;  //ȫ��֡������
        int m_cnt;  //����֡������
        bool m_bouns;   //�Ƿ�Ӧ������
        bool m_invi;    //Boss�Ƿ��޵�
        BossConversation* m_bossConversation;   //Boss�Ի��
        BossConversation* m_bossConversation_whenKilled;   //Boss������ĶԻ��

        bool m_live = true;    //����־
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
        int m_cnt_begin;  //��ʼʱ��֡��
        double m_fullHP;    //���ݵĵ�ǰ������HP

        int m_spellCardNum; //��������
        struct SpellCard{
            int endTime;    //�����־����
            bool isSpellCard;   //�Ƿ�Ϊ����
            double hp;    //��������ֵ
            int scPartten;  //��������ģʽ
            int bgPartten;  //����ͼ
            std::string title;  //��������
        };
        std::queue<SpellCard> m_spellCards;
        std::vector<int> m_bullets;

        //����ʱ��
        int m_endTime;  //Ӧ�������ʱ��
    public:
        ~Boss();
        inline int GetCnt(){return m_cnt;}
        void LoadRV(const std::string&,const std::string& basePath,int* cnt);
        void OnBirth();
        void OnDraw();
        void OnSCBGDraw();
        void OnNext();
        void OnConersationFinished();
        inline bool Live(){return m_live;};
        inline void KillHP(double hp){m_spellCards.front().hp-=hp;}
        inline bool IsInvi(){return m_invi;}
        inline int GetSpellCardNum(){return m_spellCardNum;}    //ȡ����ʣ����
        inline void ResetCnt(){m_cnt = 0;}
        int birthTime;

    protected:
};

#endif // BASEBOSS_H
