#include "Title.h"
#include "Tools.h"

Title::Title()
{
    SetLogicScreenSize(1280,720);
}


void Title::OnInit()
{

}

void Title::OnShow()
{
    m_actButton = 0;
    m_btn_y = 20;
    addButton("Title/Start.png",START);
    addButton("Title/Start.png",START);
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
}

void Title::addButton(const char* file, ButtonWork work)
{
    Button b;
    b.btn = work;
    b.tex = LoadPic(file);
    SDL_QueryTexture(b.tex,nullptr,nullptr,&b.dst_draw.w,&b.dst_draw.h);
    b.dst.x = 100;
    b.dst.y = m_btn_y;
    m_btn_y+=b.dst_draw.h;
    b.alpha = 255;
    m_buttons.push_back(b);
}

void Title::doButton(ButtonWork)
{

}

void Title::showButton()
{
    m_buttonState = SHOWING;
    m_btn_tmr.Reset();
}

void Title::hideButton()
{

}

void Title::onNextButton()
{

}

void Title::selectUp()
{

}

void Title::selectDown()
{

}

void Title::selectEnter()
{

}

void Title::onDrawButton()
{
    for(Button& b:m_buttons){
        SDL_Rect dst={b.dst.x,b.dst.y,b.dst_draw.w,b.dst_draw.h};
        SDL_SetTextureAlphaMod(b.tex,b.alpha);
        SDL_RenderCopy(Snow::pRnd,b.tex,nullptr,&dst);
    }
}
