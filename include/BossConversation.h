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
    SDL_Texture* m_tex[2] = {nullptr};  //两个图层
    int m_nowTex = 0;   //当前在上边的主图层
    SDL_Rect m_dst; //当前位置
    bool m_visible;   //可见？
    bool m_act; //是否激活

    inline int getFNowTex();    //获得当前副图层

    enum{
        NONE,   //无动画
        GOIN,   //正在淡入
        GOOUT,  //正在淡出
        CHANGING    //正在切换图层
    }m_animation;
    enum{
        ACT_NONE,
        ACT_ACTING,
        ACT_UNACTING
    }m_actAnime;
    Snow::FrameTimer m_tmr;
    Snow::FrameTimer m_act_tmr;

    int m_basic_x,m_iden;   //基本x，基本移动单位1

public:
    void OnDraw();
    void OnNext();
    void Clear();
    void Load(const std::string&);  //加载
    void Act(); //激活
    void UnAct();
    void Hide();    //隐藏
    inline void SetData(int x,int identity){
        m_basic_x = x;m_iden = identity;
    };
    //基本x位置,基本移动单位（传入正一或负一）
    inline bool Finished(){return m_animation == NONE;};    //动画是否已完成
};


class BossConversation : public KeyMapAct
{
    private:
        int* m_mainCnt; //主计时器
        int m_cnt;  //独立计时器
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
