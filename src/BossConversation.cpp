#include "BossConversation.h"
#include "WSTGame.h"
#include "str2wstr.h"
#include "MathFunc.h"
#include "Player.h"
#include "BgmMgr.h"
#include "Beater.h"

#define WINDOW_YOFFSET 50
using namespace Snow;
//BossConversation* bossConversation;


void BossConversation::OnInit()
{

}

#include "Snow/Debug.h"
void BossConversation::OnShow()
{
    m_text = new ACGCross::Galgame::TextBox;
    m_text -> Init();
    m_cnt = 0;
    m_left.Clear();
    m_right.Clear();
    m_left.SetData(100,1);
    m_right.SetData(900,-1);

    /* TODO:仅1人Play */
    m_shotPressed[0] = player[0].GetShotPressed();

    m_window.Load("textWindow.png");
    m_text->ForceClear();
    m_text->SetEffectSpeed(150);
    m_text->SetRect(200,500 + WINDOW_YOFFSET,880,200);
    m_text->SetSpeed(50);
    m_text->SetColor(255,255,255);

    /* 停止玩家活动 */
    /* DEBUG:只对应单玩家 */
    for(int i = 0;i < 1;++i){
        wstg -> OnEvent(i,T_SHOT,false);
    }
    m_nextTask = PARSE;

    m_windowAnimation = SHOWING;
    m_window.SetAlpha(0);
    m_windowTmr.Reset();
}

#include <cctype>

bool BossConversation::parseLine()
{
    while(m_conversations.front()[0] == '@'){
        if(m_conversations.front() == "@end") return false;
        else{
            std::string cmd = m_conversations.front().substr(1,3);
            std::string arg = m_conversations.front().substr(5,m_conversations.front().length()-6);
            if(cmd == "tmr" || cmd == "tmd") m_waitTrg = atoi(arg.c_str());
            if(cmd == "tmd") m_oprEnable = false;

            else if(cmd == "lhl")
                m_left.Load(arg);
            else if(cmd == "lhr")
                m_right.Load(arg);
            else if(cmd == "lha"){
                if(arg[0] == 'l') {m_left.Act();}
                else if(arg[0] == 'r') {m_right.Act();}
            }
            else if(cmd == "lhu"){
                if(arg[0] == 'l') {m_left.UnAct();}
                else if(arg[0] == 'r') {m_right.UnAct();}
            }else if(cmd == "bom"){
                bgm.UseMusic(m_bgm,1);
                beater.ChangeBpm(m_bgmBpm);
                bgm.Play();
            }else if(cmd == "stp"){
                bgm.Stop(atoi(arg.c_str()));
            }
        }
        m_conversations.pop();
        if(m_conversations.empty()) return false;
    }
    std::wstring s = StringToWString(m_conversations.front());
    m_text->AddText(s);
    if(!m_conversations.empty()) m_conversations.pop();
    return true;
}

void BossConversation::LoadConversation(const std::string& s,const std::string& basePath,Mix_Chunk* bgm,double bgmBpm)
{
    m_bgm = bgm;
    m_bgmBpm = bgmBpm;

    m_basePath = basePath;
    while(!m_conversations.empty()) m_conversations.pop();
    ResFile rf;
    rf.Load(basePath + s);
    //PNT("PATH:"<<basePath +s);
    SDL_assert(rf.Success());
    Uint32 pos = 0;
    std::string line;
    while(pos < rf.Size()){
        line = GetLine(rf,pos);
        if(line.empty()) continue;
        if(line[0] == '#') continue;
        //PNT(line);
        m_conversations.push(line);
    }
}


void BossConversation::OnDraw()
{
    wstg -> OnDraw();

    m_left.OnDraw();
    m_right.OnDraw();
    m_window.OnDraw();
    m_text->OnDraw();

}

void BossConversation::OnNext()
{
    ++m_cnt;
    wstg -> OnNext();

    m_left.OnNext();
    m_right.OnNext();

    if(m_waitTrg == m_cnt) {
        m_nextTask = PARSE;
        m_text->Clear();
        m_waitTrg = -1;
        m_oprEnable = true;
    }

    m_text->OnNext();
    if(m_text->Finished() && m_windowAnimation == NONEANM){
        switch(m_nextTask){
        case PARSE:
            m_nextTask = NONE;
            if(!parseLine()){
                m_text->Clear();
                m_left.Hide();
                m_right.Hide();
                m_nextTask = RETURN;
            }
            break;
        case RETURN:
                m_windowAnimation = HIDING;
                m_windowTmr.Reset();
            break;
        default:
            break;
        };
    }

    if(m_windowAnimation == SHOWING){
        float per = ACGCross::ArcFunc(float(m_windowTmr.GetTimer())/500);
        if(per != -1){
            m_window.SetAlpha(128*per);
            m_window.SetPos(175,WINDOW_YOFFSET + int(580-100*per));
        }else{
            m_window.SetAlpha(128);
            m_window.SetPos(175,WINDOW_YOFFSET + 480);
            m_windowAnimation = NONEANM;
        }
    }else if(m_windowAnimation == HIDING){
        float per = ACGCross::ArcFunc(float(m_windowTmr.GetTimer())/500);
        if(per != -1){
            m_window.SetAlpha(128-128*per);
            m_window.SetPos(175,WINDOW_YOFFSET + int(580-100*(1-per)));
        }else{
            if(!m_right.Finished() || !m_left.Finished()) return;
            m_window.SetAlpha(128-128);
            m_windowAnimation = NONEANM;
            m_nextTask = NONE;
            if(m_bossObj) m_bossObj -> OnConersationFinished();
            else wstg -> StageClear();
            Return();
        }
    }
}

void BossConversation::OnEvent(int p, Key k, bool b)
{
    if(k == T_SHOT) m_shotPressed[p] = b;
    if(k == T_UP || k == T_DOWN || k == T_LEFT || k == T_RIGHT || k == T_SLOW || k == T_PAUSE || k == T_ESC)
        wstg -> OnEvent(p,k,b);

    if(!m_oprEnable) return;
    else if((k == T_ENTER || k == T_SHOT) && b && m_nextTask == NONE && m_text->Finished()){
        m_nextTask = PARSE;
        m_text->Clear();
    }
    else if(k == T_SHOT && b && m_nextTask == NONE && !m_text->Finished()){
        m_text->StopSpeak();
    }
}

void BossConversation::OnHide()
{
    /* TODO:仅1P */
    if(m_shotPressed[0]) player[0].OnEvent(T_SHOT,true);
    delete m_text;
    delete this;
}

