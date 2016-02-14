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
#include "SCClock.h"
using namespace Snow;

void Boss::allocBgmAttackTime()
{
    int baseOne = 0;
    if(m_halfLastBgmBlock < m_cnt) baseOne = 1;
    m_endTime = m_bgmBlocks[baseOne + m_spellCards.front().useBGMBlock - 1];
}


void Boss::loadBgmBlocks(const std::string& path)
{
    ResFile r;
    r.Load(path);
    Uint32 pos = 0;
    std::string s;
    while(pos < r.Size()){
        s = GetLine(r,pos);
        if(s[0] == '-') break;
        else m_bgmBlocks.push_back(atoi(s.c_str()));
    }
}

void Boss::LoadRV(const std::string& s,const std::string& basePath,int* cnt)
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
    m_conversation = r.Str("CONVERSATION");
    m_conversation_whenKilled = r.Str("CONV_WHEN_KILLED");
    m_basePath = basePath;

    //BGM参数
    loadBgmBlocks(basePath + r.Str("BGM_BLK"));

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
        csv.PopInt(boolTmp);
        sc.isSpellCard = boolTmp;
        if(sc.isSpellCard) ++m_spellCardNum;
        sc.useBGMBlock = 0;
        csv.PopInt(sc.useBGMBlock);
        if(sc.useBGMBlock == 0) continue;
        csv.PopFloat(sc.hp);
        csv.PopStr(sc.title);
        csv.PopInt(sc.scPartten);
        csv.PopInt(sc.bgPartten);
        PNT("Boss CSV:"<<sc.useBGMBlock<<" "<<sc.hp<<" "<<sc.isSpellCard);
        m_spellCards.push(sc);
    }while(csv.NextLine());
    if(!m_conversation.empty()){
        //Load Bgm
        Mix_Chunk* bgm;
        double bpm;
        if(!r.Str("BGM_SND").empty()){
            ResFile rf;
            rf.Load(basePath + r.Str("BGM_SND"));
            bgm = Mix_LoadWAV_RW(rf,rf.Size());
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
    m_halfLastBgmBlock = (m_bgmBlocks[0])/2;
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

    //乐句时间流动
    if(m_cnt >= m_bgmBlocks[0]){
        m_lastBgmBlock = m_bgmBlocks[0];
        m_bgmBlocks.pop_front();
        if(m_bgmBlocks.size() >= 1){
            m_halfLastBgmBlock = (m_lastBgmBlock + m_bgmBlocks[0])/2;
        }else m_halfLastBgmBlock = m_lastBgmBlock+1500;
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
            allocBgmAttackTime();
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
        (scPartten[m_spellCards.front().scPartten])(this,m_cnt,*m_mainCnt - m_cnt_begin,m_imageUsing,m_x,m_y,m_spd,m_aspd,m_angle,m_spellCards.front().hp,m_bullets);
    }
    PNT("BOSS FRAME:"<<m_cnt);
}

void Boss::OnConersationFinished()
{
    m_collEnable = true;
    m_cnt_begin = -1;
    gameUI.SetSpellCard(m_spellCards.front().title);
    m_conversation.clear();
    if(m_bossConversation) scClock.Show();
    allocBgmAttackTime();
}

typedef void(*SCBg)(int cnt);
extern SCBg scbgs [];
void Boss::OnSCBGDraw()
{
    if(!m_collEnable || m_spellCards.empty()) return;
    if(m_live && m_spellCards.front().bgPartten != -1){
        (*scbgs[m_spellCards.front().bgPartten])(*m_mainCnt-m_cnt_begin);
    }
}

Boss::~Boss()
{
    for(int i = 0;i < 10;++i)
        SDL_DestroyTexture(m_images[i]);
}

