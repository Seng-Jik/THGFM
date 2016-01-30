#include "BossConversation.h"
#include "Tools.h"
#include "MathFunc.h"
using namespace Snow;
using namespace ACGCross;

inline int BossConChar::getFNowTex(){
    if(m_nowTex == 1) return 0;
    else return 1;
}

void BossConChar::OnDraw(){
    if(m_visible){
        SDL_RenderCopy(pRnd,m_tex[getFNowTex()],nullptr,&m_dst);
        SDL_RenderCopy(pRnd,m_tex[m_nowTex],nullptr,&m_dst);
    }
}
void BossConChar::OnNext()
{
    float per = float(m_tmr.GetTimer()) / 20;
    if(m_animation == GOIN){
        if(m_act){
            m_dst.x = m_basic_x + 300 * ArcFunc(per) *m_iden - 300*m_iden;
            SDL_SetTextureAlphaMod(m_tex[getFNowTex()],255*per);
        }else{
            m_dst.x = m_basic_x + 300 * ArcFunc(per)*m_iden - 400*m_iden;
            SDL_SetTextureAlphaMod(m_tex[getFNowTex()],128*per);
        }
        if(per >= 1) m_nowTex = getFNowTex();
    }

    if(m_animation == CHANGING){
        int trgAlpha;
        if(m_act) trgAlpha = 255;
        else trgAlpha = 128;
        SDL_SetTextureAlphaMod(m_tex[getFNowTex()],trgAlpha*per);
        SDL_SetTextureAlphaMod(m_tex[m_nowTex],trgAlpha - trgAlpha*per);
        if(per >= 1) m_nowTex = getFNowTex();
    }

    if(per >= 1){
        if(m_act){
            m_dst.x = m_basic_x;
            SDL_SetTextureAlphaMod(m_tex[m_nowTex],255);
        }else{
            m_dst.x = m_basic_x - 100*m_iden;
            SDL_SetTextureAlphaMod(m_tex[m_nowTex],128);
        }
    }

    if(m_actAnime== ACT_ACTING){
        //int needAct;
        //if(m_animation == NONE) needAct = m_nowTex;
        //else needAct = getFNowTex();

        float per = float(m_act_tmr.GetTimer())/20;

        if(m_animation != CHANGING) SDL_SetTextureAlphaMod(m_tex[m_nowTex],128+127*per);

        m_dst.x = m_basic_x  + 100*m_iden*ArcFunc(per) - 100*m_iden;
        if(per>=1){
            m_dst.x = m_basic_x;
            SDL_SetTextureAlphaMod(m_tex[m_nowTex],255);
            m_actAnime = ACT_NONE;
        }
    }

    if(m_actAnime== ACT_UNACTING){
        //int needAct;
        //if(m_animation == NONE) needAct = m_nowTex;
        //else needAct = getFNowTex();

        float per = float(m_act_tmr.GetTimer())/20;

        if(m_animation != CHANGING) SDL_SetTextureAlphaMod(m_tex[m_nowTex],255-127*per);

        m_dst.x = m_basic_x  - 100*m_iden*ArcFunc(per);
        if(per>=1){
            m_dst.x = m_basic_x-100*m_iden;
            SDL_SetTextureAlphaMod(m_tex[m_nowTex],128);
            m_actAnime = ACT_NONE;
        }
    }

    if(m_animation == GOOUT && m_visible){
        if(m_act){
            m_dst.x = m_basic_x + 300 * ArcFunc(1-per) *m_iden - 300*m_iden;
            SDL_SetTextureAlphaMod(m_tex[m_nowTex],255-255*per);
        }else{
            m_dst.x = m_basic_x + 300 * ArcFunc(1-per)*m_iden - 400*m_iden;
            SDL_SetTextureAlphaMod(m_tex[m_nowTex],128-128*per);
        }
        if(per >= 1){
            m_visible = false;
        }
    }

    if(per >= 1) m_animation = NONE;
}

void BossConChar::Clear()
{
    m_visible = false;
    m_act = false;
    m_nowTex = 0;
    m_animation = NONE;
}

void BossConChar::Load(const std::string& s)
{
    SDL_DestroyTexture(m_tex[getFNowTex()]);

    m_tex[getFNowTex()] = LoadPic(s.c_str());
    SDL_QueryTexture(m_tex[getFNowTex()],nullptr,nullptr,&m_dst.w,&m_dst.h);
    m_dst.y = HEIGHT - m_dst.h;
    if(m_act) m_dst.x = m_basic_x;
    else m_dst.x = m_basic_x - 100*m_iden;

    if(!m_visible){
        m_animation = GOIN;
        SDL_DestroyTexture(m_tex[m_nowTex]);
        m_tex[m_nowTex] = nullptr;
    }else{
        m_animation = CHANGING;
    }
    SDL_SetTextureAlphaMod(m_tex[getFNowTex()],0);
    m_visible = true;
    m_tmr.Reset();
}

void BossConChar::Act()
{
    m_actAnime = ACT_ACTING;
    m_act = true;
    m_act_tmr.Reset();
}

void BossConChar::UnAct()
{
    m_actAnime = ACT_UNACTING;
    m_act = false;
    m_act_tmr.Reset();
}

void BossConChar::Hide()
{
    m_animation = GOOUT;
    m_tmr.Reset();
}
