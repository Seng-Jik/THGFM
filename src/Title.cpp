#include "Title.h"
#include "Tools.h"
#include "MathFunc.h"
#include <Snow/Debug.h>

Title::Title()
{
    SetLogicScreenSize(1280,720);
}


void Title::OnInit()
{
    m_selecter.Load("Title/Selecter.png");
    m_actButton = 0;
}

void Title::OnShow()
{
    m_btn_y = 130;
    addButton("Title/Start.png",START);
    //addButton("Title/Extra.png",STARTEXTRA);
    //addButton("Title/Practice.png",PRATCICE);
    //addButton("Title/MusicRoom.png",MUSICROOM);
    addButton("Title/Rank.png",RANK);
    if(KeyMapAct::HaveGamePad()) addButton("Title/GamePadSetting.png",GAMEPADKEYSET);
    addButton("Title/Exit.png",EXIT);
    showButton();
}

void Title::OnNext()
{
    onNextButton();
}

void Title::OnDraw()
{
    onDrawButton();
}

void Title::OnHide()
{
    clearButton();
}

void Title::clearButton()
{
    for(Button& b:m_buttons)
        SDL_DestroyTexture(b.tex);
    m_buttons.clear();
    m_btn_y = 0;
}

void Title::addButton(const char* file, ButtonWork work)
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

void Title::doButton(ButtonWork b)
{
    switch(b){
    case EXIT:
        Snow::Exit(0);break;
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

void Title::showButton()
{
    m_buttonState = SHOWING;
    m_btn_tmr.Reset();
    for(Button& b:m_buttons)
            b.dst_draw.y = 150;
    m_selecter_trg = m_buttons[m_actButton].dst.y;
}

void Title::hideButton()
{
    m_buttonState = HIDING;
    m_btn_tmr.Reset();
}

void Title::onNextButton()
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
            m_buttonState = NORMAL;
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
        }
    }
}

void Title::selectUp()
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

void Title::selectDown()
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

void Title::setActButton(int i){
    int x;
    m_selecter.GetPos(x,m_selecter_old);
    m_actButton=i;
    m_selecter_trg = m_buttons[m_actButton].dst.y;
    m_buttonState = SELECTER_MOVING;
    m_btn_tmr.Reset();
}

void Title::selectEnter()
{
    hideButton();
}

void Title::onDrawButton()
{
    for(Button& b:m_buttons){
        //SDL_Rect dst={b.dst.x,b.dst_draw.y,b.dst_draw.w,b.dst_draw.h};
        SDL_SetTextureAlphaMod(b.tex,b.alpha);
        SDL_RenderCopy(Snow::pRnd,b.tex,nullptr,&b.dst_draw);
    }
    m_selecter.OnDraw();
}

void Title::OnEvent(int, Key k, bool b)
{
    if(m_buttonState == NORMAL || m_buttonState == SELECTER_MOVING){
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
            case T_ESC:
                showButton();
            }
        }
    }
}

