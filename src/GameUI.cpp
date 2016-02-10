#include "GameUI.h"
#include "Tools.h"
#include "WSTGame.h"
#include "str2wstr.h"
#include <cmath>
#include "Snow/Debug.h"

GameUI gameUI;

void GameUI::Init()
{
    m_heart = LoadPic("GameUI/Life.png");
    m_bomb = LoadPic("GameUI/Bomb.png");

    m_bossOpened = false;
    m_showSC = false;

    m_bossHPLineShell = LoadPic("GameUI/BossHPLineShell.png");
    m_bossHPLine = LoadPic("GameUI/BossHPLine.png");

    m_spellCardStar = LoadPic("GameUI/SpellCardStar.png");
    m_scName_animation = m_scName_show = false;

    m_font.Open("FangZhengHei.ttf",48);
    m_bossHPLineAlpha = 0;
    SDL_SetTextureBlendMode(m_bossHPLine,SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(m_bossHPLineShell,SDL_BLENDMODE_BLEND);
}

void GameUI::OnDraw()
{
    SDL_Rect r = {0,0,32,32};
    for(int i = 0;i < m_life-1;++i){
        SDL_RenderCopy(Snow::pRnd,m_heart   ,nullptr,&r);
        r.x+=32;
    }
    r.x = 0;
    r.y = HEIGHT - 32;
    for(int i = 0;i < m_bombNum && i<10;++i){
        SDL_RenderCopy(Snow::pRnd,m_bomb,nullptr,&r);
        r.x+=32;
    }

    if(m_bossOpened){

        /* Boss 剩余符卡数表示 */
        r.x = WIDTH-32;
        r.y = HEIGHT-32;
        r.w = r.h = 32;
        for(int i = 0;i < stage.GetBoss()->GetSpellCardNum()-1;++i){
            SDL_RenderCopy(Snow::pRnd,m_spellCardStar,nullptr,&r);
            r.x-=32;
        }
    }

    /* SpellCard Name */
    if(m_scName_show)
        m_scName.OnDraw();

    /* Boss 血条 */
    SDL_SetTextureAlphaMod(m_bossHPLine,255*m_bossHPLineAlpha);
    SDL_SetTextureAlphaMod(m_bossHPLineShell,255*m_bossHPLineAlpha);
    SDL_Rect rs = {600,32,639,32};
    SDL_RenderCopy(Snow::pRnd,m_bossHPLineShell,nullptr,&rs);
    rs.w = int(639*m_showSCHP);
    SDL_RenderCopy(Snow::pRnd,m_bossHPLine,nullptr,&rs);
}

void GameUI::OnNext()
{
    if(m_rebirth){
        m_rebirth --;
        if(!m_rebirth){
            if(m_life>0) player[0].Birth();
            else wstg->Pause();
        }
    }

    if(m_bossOpened && m_bossHPLineShow){
        if(fabs(m_showSCHP - m_scHP) < 0.1)
            m_showSCHP = m_scHP;
        else{
            if(m_showSCHP>m_scHP)
                m_showSCHP -= 0.05;
            else if(m_showSCHP<m_scHP)
                m_showSCHP += 0.05;
        }
    }

    if(m_scName_animation)
    {
        //use the flames as timer
        //tmp_y is useless
        //int tmp_x, tmp_y;
        int tmp_y;
        //a means alpha
        float tmp_a;
        //m_scName.GetPos(tmp_x, tmp_y);
        //if(tmp_x <= END_X)
        //{
            //calculate and apply
        if(!m_scName_downAnimation){
            if(m_scNameTime++ < 60)
            {
                float per = ACGCross::ArcFunc(m_scNameTime / 30.0f);
                if(per == -1) per = 1;
                tmp_a =  per *SC_A;
                //tmp_x = ACGCross::ArcFunc(m_scNameTime / (float)SC_TIME) * (END_X - sc_x) + sc_x;
                //tmp_y = ACGCross::ArcFunc(m_scNameTime / (float)SC_TIME) * (END_Y - SC_Y) + SC_Y;
                //m_scName.SetPos(sc_x , tmp_y);
                m_scName.SetAlpha(Uint8(tmp_a));
            }
            else{
                doScDownAnimation();
            }
        }else{
            if(m_scNameTime++ < SC_TIME-1){
                tmp_y = ACGCross::ArcFunc(m_scNameTime / (float)SC_TIME) * (END_Y - SC_Y) + SC_Y;
                m_scName.SetPos(sc_x , tmp_y);
            }
            else
            m_scName_animation = false;
        }
        //}
        //animation finished
    }

    if(m_bossHPLineShow){
        if(m_bossHPLineAlpha < 1){
            m_bossHPLineAlpha += 0.05;
            if(m_bossHPLineAlpha > 0.99)
                m_bossHPLineAlpha = 1;
        }
    }else{
        if(m_bossHPLineAlpha > 0){
            m_bossHPLineAlpha -= 0.05;
            if(m_bossHPLineAlpha < 0.01)
                m_bossHPLineAlpha = 0;
        }
    }
}

void GameUI::OnGetLife()
{
    m_life++;
}

void GameUI::OnPlayerBeKilled()
{
    m_life--;
    m_rebirth = 20;
    m_bombNum = 3;
}

void GameUI::OnGetBomb()
{
    m_bombNum++;
}

void GameUI::OnUseBomb()
{
    m_bombNum--;
}

void GameUI::OpenBoss()
{
    m_bossOpened = true;
    m_showSCHP = 0;
    m_bossHPLineAlpha = 0;
    m_bossHPLineShow = false;
}

void GameUI::CloseBoss()
{
    m_bossOpened = false;
}

int sc_x;
void GameUI::SetSpellCard(const std::string& scName)
{
    PNT("DBG BEG");
    if(scName.empty()){
        PNT("DBG PNT 0");
        m_scName.Clear();
        PNT("DBG PNT 1");
        m_scName_show = false;
    }else{
        //load the name of Spellcard and Initilize the value
        int h;
        PNT("DBG PNT 2");
        m_scName.Load(m_font,StringToWString(scName),true);
        PNT("DBG PNT 3");
        m_scName.GetSize(sc_x,h);
        sc_x = WIDTH - sc_x-34;
        PNT("DBG PNT 4");
        m_scName.SetAlpha(0);
        PNT("DBG PNT 5");
        m_scName_show = true;
        m_scNameTime = 0;
        m_scName_animation = true;
        m_scName_downAnimation = false;
        m_scName.SetPos(sc_x, SC_Y);
        PNT("DBG PNT 6");
    }
    PNT("DBG END");
}

void GameUI::doScDownAnimation()
{
    m_scName_tmr.Reset();
    m_scName_downAnimation = true;
    m_scNameTime = 0;
}
