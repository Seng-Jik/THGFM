#include "Title/TitleMenu.h"
#include "Tools.h"
#include "MathFunc.h"
#include "BgmMgr.h"
#include <Snow/Debug.h>

TitleMenu titleMenu;

TitleMenu::TitleMenu()
{
    SetLogicScreenSize(1280,720);
}


void TitleMenu::OnInit()
{
    m_selecter.Load("Title/Selecter.png");
    m_actButton = 0;
    m_bgp = LoadPic("Title/bgp.png");
}

void TitleMenu::OnShow()
{
    //bgm.LoadMusic("Title/Title.wav",0);
    //bgm.Play();
    m_btn_y = 130;
    addButton("Title/Start.png",START);
    //addButton("Title/Extra.png",STARTEXTRA);
    //addButton("Title/Practice.png",PRATCICE);
    //addButton("Title/MusicRoom.png",MUSICROOM);
    addButton("Title/Rank.png",RANK);
    if(KeyMapAct::HaveGamePad()) addButton("Title/GamePadSetting.png",GAMEPADKEYSET);
    addButton("Title/Exit.png",EXIT);
    showButton();
    m_selecter_alpha_downing = true;
}

void TitleMenu::OnNext()
{
    onNextButton();
}

void TitleMenu::OnDraw()
{
    SDL_RenderCopy(Snow::pRnd,m_bgp,nullptr,nullptr);
    onDrawButton();
}

void TitleMenu::OnHide()
{
    clearButton();
}

void TitleMenu::clearButton()
{
    for(Button& b:m_buttons)
        SDL_DestroyTexture(b.tex);
    m_buttons.clear();
    m_btn_y = 0;
}

void TitleMenu::addButton(const char* file, ButtonWork work)
{
    Button b;
    b.btn = work;
    b.tex = LoadPic(file);
    SDL_QueryTexture(b.tex,nullptr,nullptr,&b.dst_draw.w,&b.dst_draw.h);
    b.dst_draw.x = b.dst.x = 0;
    b.dst.y = m_btn_y;
    m_btn_y+=b.dst_draw.h;
    b.alpha = 255;
    m_buttons.push_back(b);
}

void TitleMenu::doButton(ButtonWork b)
{
    switch(b){
    case EXIT:
        Snow::Exit(0);
        break;
    case START:
        break;
    case STARTEXTRA:
        break;
    case PRATCICE:
        break;
    case RANK:
        break;
    case GAMEPADKEYSET:
        break;
    case MUSICROOM:
        break;
    }
}

void TitleMenu::showButton()
{
    m_buttonState = SHOWING;
    m_btn_tmr.Reset();
    for(Button& b:m_buttons)
            b.dst_draw.y = 150;
    m_selecter_trg = m_buttons[m_actButton].dst.y;
    m_selecterAlpha = 255;
}

void TitleMenu::hideButton()
{
    m_buttonState = HIDING;
    m_btn_tmr.Reset();
}

void TitleMenu::onNextButton()
{
    if(m_buttonState == SHOWING){
        float per = ACGCross::ArcFunc(float(m_btn_tmr.GetTimer())/500);
        if(per == -1){
            for(Button& b:m_buttons){
                b.dst_draw.y = b.dst.y;
                b.alpha = 192;
            }
            m_buttonState = NORMAL;
            m_selecter.SetPos(0,m_selecter_trg);
            m_selecter.SetAlpha(255);
        }
        else{
            for(Button& b:m_buttons){
                b.dst_draw.y = 150 + (per*b.dst.y - 150);
                b.alpha = 192*per;
            }
            //m_selecter.SetPos(0,int(per*(m_selecter_trg - 50)+50));
            m_selecter.SetPos(0,m_selecter_trg);
            m_selecter.SetAlpha(255*per);
        }
    }

    else if(m_buttonState == HIDING){
        float per = ACGCross::ArcFunc(float(m_btn_tmr.GetTimer())/500);
        if(per == -1){
            for(Button& b:m_buttons){
                b.dst_draw.y = 150;
                b.alpha = 0;
            }
            m_buttonState = HIDDEN;
            m_selecter.SetAlpha(0);
            //Do Work
            doButton(m_buttons[m_actButton].btn);
        }
        else if(per >0 && per <1) for(Button& b:m_buttons){
            b.dst_draw.y = 150 + ((1-per)*b.dst.y - 150);
            b.alpha = 192-192*per;
            //m_selecter.SetPos(0,int((1-per)*(m_selecter_trg - 50)+50));
            m_selecter.SetAlpha(255-255*per);
        }
    }

    else if(m_buttonState == SELECTER_MOVING){
        float per = ACGCross::ArcFunc(float(m_btn_tmr.GetTimer())/500);
        if(per == -1){
            m_selecter.SetPos(0,int(m_selecter_trg));
            m_buttonState = NORMAL;
        }else{
            m_selecter.SetPos(0,int(per*(m_selecter_trg - m_selecter_old))+m_selecter_old);
            if(m_selecterAlpha <= 250) m_selecterAlpha+= 5;
            m_selecter.SetAlpha(m_selecterAlpha);
        }
    }

    if((m_buttonState == SELECTER_MOVING || m_buttonState == NORMAL)&& m_buttonState != HIDING && m_buttonState != HIDDEN){
        if(m_selecter_alpha_downing){
            m_selecterAlpha -= 1;
            if(m_selecterAlpha < 128) m_selecter_alpha_downing = false;
        }else{
            m_selecterAlpha += 1;
            if(m_selecterAlpha > 245) m_selecter_alpha_downing = true;
        }
        m_selecter.SetAlpha(m_selecterAlpha);
    }

    if(m_buttonState == SELECTER_FLASHING){
        for(int i = 0;i<7;++i){
            if(Snow::pRnd.GetFrameCount()%4>=2){
                m_selecterAlpha = 255;
            }else{
                m_selecterAlpha = 0;
            }
        }
        m_selecter.SetAlpha(m_selecterAlpha);
        if(m_btn_tmr.GetTimer() >= 200){
            hideButton();
        }
    }
}

void TitleMenu::selectUp()
{
    if(m_actButton > 0){
        int x;
        m_selecter.GetPos(x,m_selecter_old);
        --m_actButton;
        m_selecter_trg = m_buttons[m_actButton].dst.y;
        m_buttonState = SELECTER_MOVING;
        m_btn_tmr.Reset();
    }
}

void TitleMenu::selectDown()
{
    if(m_actButton < m_buttons.size() -1){
        int x;
        m_selecter.GetPos(x,m_selecter_old);
        ++m_actButton;
        m_selecter_trg = m_buttons[m_actButton].dst.y;
        m_buttonState = SELECTER_MOVING;
        m_btn_tmr.Reset();
    }
}

void TitleMenu::setActButton(int i){
    int x;
    m_selecter.GetPos(x,m_selecter_old);
    m_actButton=i;
    m_selecter_trg = m_buttons[m_actButton].dst.y;
    m_buttonState = SELECTER_MOVING;
    m_btn_tmr.Reset();
}

void TitleMenu::selectEnter()
{
    if(m_buttons[m_actButton].btn == EXIT) bgm.Stop(500);
    m_buttonState = SELECTER_FLASHING;
    m_btn_tmr.Reset();
}

void TitleMenu::onDrawButton()
{
    for(Button& b:m_buttons){
        //SDL_Rect dst={b.dst.x,b.dst_draw.y,b.dst_draw.w,b.dst_draw.h};
        SDL_SetTextureAlphaMod(b.tex,b.alpha);
        SDL_RenderCopy(Snow::pRnd,b.tex,nullptr,&b.dst_draw);
    }
    m_selecter.OnDraw();
}

void TitleMenu::OnEvent(int, Key k, bool b)
{
    if(m_buttonState == NORMAL || m_buttonState == SELECTER_MOVING){// || m_buttonState == HIDDEN){
        if(b){
            switch(k){
            case T_UP:
                selectUp();
                break;
            case T_DOWN:
                selectDown();
                break;
            case T_ENTER:
            case T_SHOT:
                selectEnter();
                break;
            /* Debug */
            //case T_ESC:
                //showButton();
            }
        }
    }
}

