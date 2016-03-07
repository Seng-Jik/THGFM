#include "CharData.h"
#include "MathFunc.h"
#include "Tools.h"
#include "WSTGame.h"
#include "CollWorld.h"
#include "PlayerBullet.h"
#include "EffectMgr.h"
Marisa marisa;
void Marisa::Init()
{
    m_marisa_boom = LoadPic("Player/Marisa/boom.png");
    SDL_SetTextureAlphaMod(m_marisa_boom,192);
    SDL_SetTextureBlendMode(m_marisa_boom,SDL_BLENDMODE_ADD);
    m_r.w = 1280;
    m_mask = 0;
}

bool Marisa::BoomOnNext(Player* p)
{
    double x,y;
    p->GetPos(x,y);
    m_r.x = x;
    m_r.y = y;
    if(m_tmr.GetTimer() <= 30){
        float per = ACGCross::ArcFunc(float(m_tmr.GetTimer()) / 30);
        m_r.h = per*560;
        m_mask = per;
        if(m_mask == -1) m_mask = 1;
        if(per == -1) m_r.h = 560;
    }else if(m_tmr.GetTimer() <= 300){
        m_r.h = 560;
    }else if(m_tmr.GetTimer() <= 330){
        float per = ACGCross::ArcFunc(float(m_tmr.GetTimer()-300) / 30);
        m_r.h = 560-per*560;
        m_mask = 1-per;
        if(m_mask == -1) m_mask = 0;
        if(per == -1){
            m_r.h = 0;
            m_mask = 0;
        }
    }else return false;
    m_r.y-=m_r.h/2;
    collWorld.SetBoom(true,0,m_r.x,m_r.y,m_r.w,m_r.h);
    return true;
}

void Marisa::BoomOnDraw()
{
    SDL_SetRenderDrawBlendMode(Snow::pRnd,SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(Snow::pRnd,0,0,24,192*m_mask);
    SDL_RenderFillRect(Snow::pRnd,nullptr);
    SDL_RenderCopy(Snow::pRnd,m_marisa_boom,nullptr,&m_r);
}

void Marisa::BoomReset(Player*)
{
    m_tmr.Reset();
    wstg->GrpShake(330);
}
#include <iostream>
void Marisa::BulletInstaller(double power,int cnt,double x,double y)
{
    //TODO
    std::cout<<"marisa";
}

void Marisa::InstallDeatAnimation(int x, int y)
{
    effMgr.InstallCharBoomAnimation(x,y,255,255,255,0);
}



Reimu reimu;
void Reimu::Init()
{
    m_reimu_w = LoadPic("Player/Reimu/boom0.png");
    m_reimu_h = LoadPic("Player/Reimu/boom1.png");
    for(int i = 0;i < 2;++i){
        m_wr[i].w = 1280;
        m_wr[i].h = 200;
        m_hr[i].w = 200;
        m_hr[i].h = 720;
    }
    m_mask = 0;
}

bool Reimu::BoomOnNext(Player* p)
{
    if(m_tmr.GetTimer() < 60){
        float per = ACGCross::ArcFunc(float(m_tmr.GetTimer()) / 60);
        SDL_SetTextureAlphaMod(m_reimu_h,192*per);
        SDL_SetTextureAlphaMod(m_reimu_w,192*per);
        m_mask = per;
        if(per == -1) m_mask = 1;
    }
    else if(m_tmr.GetTimer() == 60){
        m_mask = 1;
        SDL_SetTextureAlphaMod(m_reimu_h,192);
        SDL_SetTextureAlphaMod(m_reimu_w,192);
    }else if(m_tmr.GetTimer()>=90){
        float per = ACGCross::ArcFunc(float(m_tmr.GetTimer()-90) / 60);
        SDL_SetTextureAlphaMod(m_reimu_h,192-192*per);
        SDL_SetTextureAlphaMod(m_reimu_w,192-192*per);
        m_mask = 1- per;
        if(per == -1) m_mask = 0;
        m_wr[1].y+=m_spd/2;
        m_wr[0].y-=m_spd/2;
        m_hr[0].x-=m_spd;
        m_hr[1].x+=m_spd;
        m_spd -= 0.8;
        if(m_spd <= 0.3) m_spd = 0.3;
    }else{
        m_wr[1].y+=m_spd/2;
        m_wr[0].y-=m_spd/2;
        m_hr[0].x-=m_spd;
        m_hr[1].x+=m_spd;
        m_spd -= 0.8;
        if(m_spd <= 0.3) m_spd = 0.3;
    }
    if(m_tmr.GetTimer()>=150) return false;
    collWorld.SetBoom(true,0,m_wr[0].x,m_wr[0].y,m_wr[0].w,m_wr[0].h);
    collWorld.SetBoom(true,1,m_wr[1].x,m_wr[1].y,m_wr[1].w,m_wr[1].h);
    collWorld.SetBoom(true,2,m_hr[0].x,m_hr[0].y,m_hr[0].w,m_hr[0].h);
    collWorld.SetBoom(true,3,m_hr[1].x,m_hr[1].y,m_hr[1].w,m_hr[1].h);
    return true;
}

void Reimu::BoomOnDraw()
{
    SDL_SetRenderDrawBlendMode(Snow::pRnd,SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(Snow::pRnd,48,32,0,192*m_mask);
    SDL_RenderFillRect(Snow::pRnd,nullptr);
    SDL_RenderCopy(Snow::pRnd,m_reimu_w,nullptr,&m_wr[0]);
    SDL_RenderCopy(Snow::pRnd,m_reimu_h,nullptr,&m_hr[0]);
    SDL_RenderCopyEx(Snow::pRnd,m_reimu_w,nullptr,&m_wr[1],0,nullptr,SDL_FLIP_VERTICAL);
    SDL_RenderCopyEx(Snow::pRnd,m_reimu_h,nullptr,&m_hr[1],0,nullptr,SDL_FLIP_HORIZONTAL);
}

void Reimu::BoomReset(Player* p)
{
    m_tmr.Reset();
    wstg->GrpShake(210);
    m_spd = 32;

    double x,y;
    p -> GetPos(x,y);
    for(int i = 0;i < 2;++i){
        m_wr[i].x = 0;
        m_wr[i].y = y-100;
        m_hr[i].x = x-100;
        m_hr[i].y = 0;
    }
}

void Reimu::BulletInstaller(double power,int cnt,double x,double y)
{
    if(cnt%3 == 0){
        int powerMode = int(power);
        switch(powerMode){
        case 0:
            playerBulletMgr.Add(x+40,y-12,0.5,0);
            playerBulletMgr.Add(x+40,y+12,0.5,0);
            break;
        case 1:
            playerBulletMgr.Add(x+40,y-12,0.6,0);
            playerBulletMgr.Add(x+40,y+12,0.6,0);
            break;
        case 2:
            playerBulletMgr.Add(x+40,y-12,0.7,0);
            playerBulletMgr.Add(x+40,y+12,0.7,0);
            break;
        default:
            playerBulletMgr.Add(x+40,y-32,0.4,0);
            playerBulletMgr.Add(x+40,y-12,0.4,0);
            playerBulletMgr.Add(x+40,y+12,0.4,0);
            playerBulletMgr.Add(x+40,y+32,0.4,0);
        }
    }
}

void Reimu::InstallDeatAnimation(int x, int y)
{
    effMgr.InstallCharBoomAnimation(x,y,216,4,9,0);
    effMgr.InstallCharBoomAnimation(x,y,216,32,96,7);
    effMgr.InstallCharBoomAnimation(x,y,216,4,9,14);
    //effMgr.InstallCharBoomAnimation(x,y,10,0,0,15);
}

