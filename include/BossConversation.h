#ifndef BOSSCONVERSATION_H
#define BOSSCONVERSATION_H

#include "KeyMapAct.h"
#include "TextBox.h"
#include "BgmMgr.h"
#include <queue>
#include <string>

class Boss;
class BossConChar{
private:
    SDL_Texture* m_tex[2] = {nullptr};  //����ͼ��
    int m_nowTex = 0;   //��ǰ���ϱߵ���ͼ��
    SDL_Rect m_dst; //��ǰλ��
    bool m_visible;   //�ɼ���
    bool m_act; //�Ƿ񼤻�

    inline int getFNowTex();    //��õ�ǰ��ͼ��

    enum{
        NONE,   //�޶���
        GOIN,   //���ڵ���
        GOOUT,  //���ڵ���
        CHANGING    //�����л�ͼ��
    }m_animation;
    enum{
        ACT_NONE,
        ACT_ACTING,
        ACT_UNACTING
    }m_actAnime;
    Snow::FrameTimer m_tmr;
    Snow::FrameTimer m_act_tmr;

    int m_basic_x,m_iden;   //����x�������ƶ���λ1

public:
    void OnDraw();
    void OnNext();
    void Clear();
    void Load(const std::string&);  //����
    void Act(); //����
    void UnAct();
    void Hide();    //����
    inline void SetData(int x,int identity){
        m_basic_x = x;m_iden = identity;
    };
    //����xλ��,�����ƶ���λ��������һ��һ��
    inline bool Finished(){return m_animation == NONE;};    //�����Ƿ������
};


class BossConversation : public KeyMapAct
{
    private:
        int* m_mainCnt; //����ʱ��
        int m_cnt;  //������ʱ��
        Boss* m_bossObj;
    public:
        void SetPtrs(int* cnt,Boss* b){m_mainCnt = cnt;m_bossObj = b;}
        void OnInit();
        void OnShow();
        void OnDraw();
        void OnHide();
        void OnNext();
        void OnEvent(int,Key,bool);
        void LoadConversation(const std::string& s,const std::string& basePath,Bgm* bgm,double bgmBpm);
    protected:
    private:
        Bgm* m_bgm;
        double m_bgmBpm;

        ACGCross::Galgame::TextBox* m_text;
        std::string m_basePath;
        std::queue<std::string> m_conversations;
        bool parseLine();
        enum {
            PARSE,
            NONE,
            RETURN
        }m_nextTask;

        enum{
            NONEANM,
            SHOWING,
            HIDING
        } m_windowAnimation;

        bool m_shotPressed[2];

        Snow::Sprite m_window;
        Snow::Timer m_windowTmr;
        bool m_oprEnable = true;
        int m_waitTrg = -1;

        BossConChar m_left,m_right;
};

//extern BossConversation* bossConversation;

#endif // BOSSCONVERSATION_H
