#include "Boss.h"
#include "Snow/Debug.h"
#include "Tools.h"
#include "BossConversation.h"
#include "ShotParttenAPI.h"
#include <cmath>
#include "ItemMgr.h"
#include "CollWorld.h"
#include "GameUI.h"
#include "WSTGame.h"
#include "Player.h"
#include "SCClock.h"
typedef void(*SCBg)(int cnt,Snow::Bundle<256>&);
extern SCBg scbgs [];
using namespace Snow;

void Boss::LoadRV(const std::string& s,const std::string& basePath,int* cnt,const std::string& playerChar)
{
    m_cnt = 0;
    m_spellCardNum = 0;
    m_mainCnt = cnt;
    m_bouns = true;
    PNT("Boss::LoadRV:"<<s);
    m_invi = false;
    ResVal r;
    r.Load(s);

    //基本参数
    m_midway = r.Int("MIDWAY_BOSS");
    m_conversation = r.Str("CONVERSATION_"+playerChar);
    m_conversation_whenKilled = r.Str("CONV_WHEN_KILLED_"+playerChar);
    m_basePath = basePath;

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
    SpellCard sc;
    csv.LoadCSV(basePath+r.Str("SC_CSV"));
    csv.NextLine();
    do{
        int boolTmp;
        csv.PopInt(boolTmp);
        if(boolTmp == 2) continue;
        PNT("POP A BOSS SKILL.");
        sc.isSpellCard = boolTmp;
        if(sc.isSpellCard) ++m_spellCardNum;
        sc.timeLimit = -1;
        csv.PopInt(sc.timeLimit);
        csv.PopFloat(sc.hp);
        csv.PopStr(sc.title);
        csv.PopInt(sc.scPartten);
        csv.PopInt(sc.bgPartten);
        if(sc.bgPartten!= -1) {
            sc.scBgData = new Snow::Bundle<256>;
            sc.scBgData->ResetPtr();
            (*scbgs[sc.bgPartten])(-1,*sc.scBgData);
        }
        m_spellCards.push(sc);
    }while(csv.NextLine());
    if(!m_conversation.empty()){
        //Load Bgm
        Bgm* bgm;
        float bpm;
        if(!r.Str("BGM_SND").empty()){
            bgm = new Bgm(basePath + r.Str("BGM_SND"));
            bpm = r.Float("BGM_BPM");
        }else{
            bgm = nullptr;
            bpm = 1;
        }

        m_collEnable = false;
        m_bossConversation = new BossConversation;
        m_bossConversation ->LoadConversation(m_conversation,m_basePath,bgm,bpm);
        m_bossConversation ->SetPtrs(m_mainCnt,this);
    }else{
        m_collEnable = true;
        m_bossConversation = nullptr;
    }
    m_bossConversation_whenKilled = nullptr;
    if(!m_conversation_whenKilled.empty()){
        m_bossConversation_whenKilled = new BossConversation;
        m_bossConversation_whenKilled -> LoadConversation(m_conversation_whenKilled,m_basePath,nullptr,0);
        m_bossConversation_whenKilled -> SetPtrs(m_mainCnt,nullptr);
    }
}

void Boss::OnBirth()
{
    m_cnt_begin = -1;
    collWorld.SetBossObj(this);
    //启动时启动对话系统
    if(m_bossConversation){
        Call(m_bossConversation);
    }else OnConersationFinished();
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
    ++m_cnt;

    if(*m_mainCnt%6 == 0){
        m_imageUsing++;
        if(!m_images[m_imageUsing]) m_imageUsing = 0;
    }
    gameUI.UpdateSCHP(m_spellCards.front().hp/m_fullHP);
    if(m_invi){
        m_invi = player[0].Booming();
    }
    if(m_cnt_begin == -1){
        m_cnt_begin = *m_mainCnt;
    }
    m_x -= m_spd * cos(m_angle);
    m_y -= m_spd * sin(m_angle);
    m_spd += m_aspd;
    if(!m_invi) collWorld.SetBossEnemy(m_collEnable,m_x+m_images_w/2,m_y+m_images_h/2,m_images_h/2);
    if(m_firsShow && m_spd <= 0){
        m_aspd = m_spd = 0;
        m_firsShow = false;
        if(!m_bossConversation) scClock.Show();
        PNT("SHOW CLOCK BY ONNEXT");
    }
    if(m_firsShow) return;
    scClock.SetTime(m_endTime - m_cnt);

    //当符卡死亡后
    if(((m_spellCards.front().hp<=0 && m_fullHP > 0) || m_cnt >= m_endTime) && m_collEnable){
        if(m_bouns && m_spellCards.front().isSpellCard){
            if(rand()%10 <= 1) itemMgr.AddItem(FULLPOWER,10,m_x,m_y,1);
            else itemMgr.AddItem(BOMB,10,m_x,m_y,1);
            itemMgr.AddItem(SCORE,15,m_x,m_y,20);
            itemMgr.AddItem(POWER,15,m_x,m_y,10);
            gameUI.KillSpellCard();
        }
        m_cnt_begin = -1;
        bulletMgr.Clear();
        m_bullets.clear();
        if(m_spellCards.front().bgPartten != -1){
            m_spellCards.front().scBgData->ResetPtr();
            (*scbgs[m_spellCards.front().bgPartten])(-2,*m_spellCards.front().scBgData);
            delete m_spellCards.front().scBgData;
        }

        m_spellCards.pop();
        m_bouns = true;
        gameUI.UpdateSCHP(1.0);
        if(m_spellCards.empty()){
            itemMgr.AddItem(LIFE1UP,10,m_x,m_y,1);
            collWorld.SetBossEnemy(false,0,0,0);
            m_live = false;
            gameUI.CloseBoss();
            gameUI.HideHPLine();
            scClock.Hide();
            if(m_bossConversation_whenKilled){
                Call(m_bossConversation_whenKilled);
                itemMgr.GetAll(0);
            }
            PNT("Boss End");
        }else{
            if(m_spellCards.front().bgPartten != -1) wstg -> FadeInBossSpellCardBgp();
            m_endTime = m_spellCards.front().timeLimit + m_cnt;
            if(m_spellCards.front().isSpellCard)
            {
                m_spellCardNum--;
                gameUI.SetSpellCard(m_spellCards.front().title);
            }
            m_fullHP = m_spellCards.front().hp;
            if(m_fullHP > 0) gameUI.ShowHPLine();
            else gameUI.HideHPLine();
            gameUI.ResetHPLine();
            m_invi = player[0].Booming();
            if(m_invi) collWorld.SetBossEnemy(false,m_x+m_images_w/2,m_y+m_images_h/2,m_images_h/2);
        }
        PNT("Spell Card End");
    }
    else if(m_collEnable){
        m_scParttenData.ResetPtr();
        (scPartten[m_spellCards.front().scPartten])(this,m_cnt,*m_mainCnt - m_cnt_begin,m_imageUsing,m_x,m_y,m_spd,m_aspd,m_angle,m_spellCards.front().hp,m_bullets,m_scParttenData);
    }
}

void Boss::OnConersationFinished()
{
    m_collEnable = true;
    m_cnt_begin = -1;
    gameUI.SetSpellCard(m_spellCards.front().title);
    m_conversation.clear();
    if(m_bossConversation) scClock.Show();
    m_endTime = m_spellCards.front().timeLimit + m_cnt;
}


void Boss::OnSCBGDraw()
{
    if(!m_collEnable || m_spellCards.empty()) return;
    if(m_live && m_spellCards.front().bgPartten != -1 && m_cnt_begin != -1){
        m_spellCards.front().scBgData->ResetPtr();
        (*scbgs[m_spellCards.front().bgPartten])(*m_mainCnt-m_cnt_begin,*m_spellCards.front().scBgData);
    }
}

Boss::~Boss()
{
    for(int i = 0;i < 10;++i)
        SDL_DestroyTexture(m_images[i]);
    while(!m_spellCards.empty()){
        if(m_spellCards.front().bgPartten != -1) delete m_spellCards.front().scBgData;
        m_spellCards.pop();
    }
}

