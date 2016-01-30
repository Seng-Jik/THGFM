#include "Boss.h"
#include "Snow/Debug.h"
#include "Tools.h"
#include "BossConversation.h"
#include "ShotParttenAPI.h"
#include <cmath>
#include "ItemMgr.h"
#include "CollWorld.h"
#include "GameUI.h"
#include "Player.h"
using namespace Snow;
void Boss::LoadRV(const std::string& s,const std::string& basePath,int* cnt)
{
    m_spellCardNum = 0;
    m_cnt = cnt;
    m_bouns = true;
    PNT("Boss::LoadRV:"<<s);
    m_invi = false;
    ResVal r;
    r.Load(s);

    //基本参数
    m_midway = r.Int("MIDWAY_BOSS");
    m_conversation = r.Str("CONVERSATION");
    m_basePath = basePath;

    //BGM参数

    //图像参数
    for(int i = 0;i < 10;++i){
        std::string s("IMG_FILE_");
        s+=char('0'+i);
        s = r.Str(s);
        if(!s.empty()) m_images[i] = LoadPic(basePath+s);
        else m_images[i] = nullptr;
    }
    SDL_QueryTexture(m_images[0],nullptr,nullptr,&m_images_w,&m_images_h);
    m_imageUsing = 0;
    m_spd = 10;
    m_aspd = -0.1;
    m_angle = 0.78539816339744830961566084581988;
    m_y = 500;
    m_x = WIDTH+80;
    m_firsShow = true;

    //符卡列表CSV配置
    CSVReader csv;
    csv.LoadCSV(basePath + r.Str("SC_CSV"));
    SpellCard sc;
    do{
        int boolTmp;
        sc.endTime = 0;
        csv.PopInt(boolTmp);
        sc.isSpellCard = boolTmp;
        if(sc.isSpellCard) ++m_spellCardNum;
        csv.PopInt(sc.endTime);
        if(sc.endTime == 0) continue;
        csv.PopFloat(sc.hp);
        csv.PopStr(sc.title);
        csv.PopInt(sc.scPartten);
        csv.PopInt(sc.bgPartten);
        PNT("Boss CSV:"<<sc.endTime<<" "<<sc.hp<<" "<<sc.isSpellCard);
        m_spellCards.push(sc);
    }while(csv.NextLine());
    if(!m_conversation.empty()){
        m_collEnable = false;
        bossConversation ->LoadConversation(m_conversation,m_basePath);
    }else m_collEnable = true;
}

void Boss::OnBirth()
{
    m_cnt_begin = -1;
    collWorld.SetBossObj(this);
    bossConversation ->SetPtrs(m_cnt,this);
    //启动时启动对话系统
    if(!m_conversation.empty()){
        Call(bossConversation);
        m_conversation.clear();
    }
    gameUI.OpenBoss();
    gameUI.UpdateSCHP(1.0);
    m_fullHP = m_spellCards.front().hp;
    if(m_fullHP > 0) gameUI.ShowHPLine();
}

void Boss::OnDraw()
{
    SDL_Rect r = {int(m_x),int(m_y),m_images_w,m_images_h};
    SDL_RenderCopy(pRnd,m_images[m_imageUsing],nullptr,&r);
}

void Boss::OnNext()
{
    gameUI.UpdateSCHP(m_spellCards.front().hp/m_fullHP);
    if(m_invi){
        m_invi = player[0].Booming();
    }
    if(m_cnt_begin == -1)
        m_cnt_begin = *m_cnt;
    m_x -= m_spd * cos(m_angle);
    m_y -= m_spd * sin(m_angle);
    if(!m_invi) collWorld.SetBossEnemy(m_collEnable,m_x+m_images_w/2,m_y+m_images_h/2,m_images_h/2);
    m_spd += m_aspd;
    if(m_firsShow && m_spd <= 0){
        m_aspd = m_spd = 0;
        m_firsShow = false;
    }

    //当符卡死亡后
    if(((m_spellCards.front().hp<=0 && m_fullHP > 0) || *m_cnt >= m_spellCards.front().endTime) && m_collEnable){
        if(m_bouns && m_spellCards.front().isSpellCard){
            if(rand()%10 <= 1) itemMgr.AddItem(FULLPOWER,10,m_x,m_y,1);
            else itemMgr.AddItem(BOMB,10,m_x,m_y,1);
            itemMgr.AddItem(SCORE,15,m_x,m_y,20);
            itemMgr.AddItem(POWER,15,m_x,m_y,10);
        }
        m_cnt_begin = -1;
        bulletMgr.Clear();
        m_bullets.clear();
        m_spellCards.pop();
        m_bouns = true;
        gameUI.UpdateSCHP(1.0);
        if(m_spellCards.empty()){
            itemMgr.AddItem(LIFE1UP,10,m_x,m_y,1);
            collWorld.SetBossEnemy(false,0,0,0);
            m_live = false;
            gameUI.CloseBoss();
            gameUI.HideHPLine();
            PNT("Boss End");
        }else{
            if(m_spellCards.front().isSpellCard) m_spellCardNum--;
            m_fullHP = m_spellCards.front().hp;
            if(m_fullHP > 0) gameUI.ShowHPLine();
            else gameUI.HideHPLine();
            gameUI.ResetHPLine();
            m_invi = player[0].Booming();
            if(m_invi) collWorld.SetBossEnemy(false,m_x+m_images_w/2,m_y+m_images_h/2,m_images_h/2);
        }
        PNT("Spell Card End");
    }
    else if(m_collEnable) (scPartten[m_spellCards.front().scPartten])(this,*m_cnt-m_cnt_begin,*m_cnt,m_imageUsing,m_x,m_y,m_spd,m_aspd,m_angle,m_spellCards.front().hp,m_bullets);
}

void Boss::OnConersationFinished()
{
    m_collEnable = true;
    m_cnt_begin = -1;
}

typedef void(*SCBg)(int cnt);
extern SCBg scbgs [];
void Boss::OnSCBGDraw()
{
    if(!m_collEnable || m_spellCards.empty()) return;
    if(m_live && m_spellCards.front().bgPartten != -1){
        (*scbgs[m_spellCards.front().bgPartten])(*m_cnt-m_cnt_begin);
    }
}

Boss::~Boss()
{
    for(int i = 0;i < 10;++i)
        SDL_DestroyTexture(m_images[i]);
}
