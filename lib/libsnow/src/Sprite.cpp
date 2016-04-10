#include "Snow/Globals.h"
#include "Snow/Sprite.h"
#include "Snow/Font.h"
#include <string>
#include "Snow/Misc.h"
#include "Snow/Debug.h"
#include "Snow/RndPtr.h"
using namespace std;
using namespace Snow;


void Sprite::Load(const std::string& fileName)
{
    Clear();
    m_file.Load(fileName);
    m_tex = IMG_LoadTexture_RW(pRnd,(SDL_RWops*)m_file,m_file.Size());
    int w,h;
    GetSize(w,h);
    SetSrc(0,0,w,h);
    SetZoom(w,h);
}

void Sprite::Load(Font& font,const wstring& text,bool withShadow)
{
    Clear();
    //SDL_Color c = {0,0,0};
    SDL_Surface* pSur = TTF_RenderUNICODE_Blended_Wrapped((TTF_Font*)font,(const Uint16*)text.c_str(),m_text_color,15);

    if(withShadow && pSur != nullptr) DrawTextOutLine(pSur);

    Load(pSur);
    if(pSur != nullptr) SDL_FreeSurface(pSur);
    //PNT(SDL_GetError());
    //PNT(TTF_GetError());
}

void Sprite::SetRenderTextColor(Uint8 r,Uint8 g,Uint8 b,Uint8 a){
    m_text_color.r = r;
    m_text_color.g = g;
    m_text_color.b = b;
    m_text_color.a = a;
}

void Sprite::Load(SDL_Surface* pSur)
{
    Clear();
    int w,h;
    if(pSur != nullptr) {
        m_tex = SDL_CreateTextureFromSurface(pRnd,pSur);
        GetSize(w,h);
        SetSrc(0,0,w,h);
        SetZoom(w,h);
    }else Clear();
}

void Sprite::Load(SDL_Texture* pTex)
{
    Clear();
    m_tex = pTex;
}

Sprite::Sprite()
{
    m_tex = nullptr;
    m_roll = false;
    m_roll_flp = SDL_FLIP_NONE;
}

void Sprite::OnDraw()
{
    if(m_tex == nullptr) return;
    else if(!m_roll) SDL_RenderCopy(pRnd, m_tex, &m_src,&m_dst);
    else SDL_RenderCopyEx(pRnd,m_tex,&m_src,&m_dst,m_roll_ang,&m_roll_pnt,m_roll_flp);
}

void Sprite::Clear()
{
    if(m_tex != nullptr)
    {
        SDL_DestroyTexture(m_tex);
        m_tex = nullptr;
        //PNT("SpriteClear:"<<this<<endl);
    }
    m_file.Free();
}

void Sprite::SetSrc(int x, int y, int w, int h)
{
    m_src = {x, y, w, h};
}

void Sprite::SetPos(int x, int y)
{
    m_dst.x = x;
    m_dst.y = y;
}

void Sprite::SetPos(char base,int x,int y)
{
    Pos(m_dst.x,m_dst.y,base,x,y);
}

void Sprite::SetPos(float x,float y)
{
    Pos(m_dst.x,m_dst.y,x,y);
}

void Sprite::SetZoom(int w,int h)
{
    m_dst.w = w;
    m_dst.h = h;
}

void Sprite::SetAlpha(Uint8 alpha)
{
    SDL_SetTextureAlphaMod(m_tex,alpha);
}

bool Sprite::InRect(int x, int y)
{
    return x >= m_dst.x && x <= m_dst.x + m_dst.w && y >= m_dst.y && y <= m_dst.y + m_dst.h;
}

void Sprite::SetColor(Uint8 R,Uint8 G,Uint8 B)
{
    SDL_SetTextureColorMod(m_tex,R,G,B);
}

void Sprite::SetBlend(SDL_BlendMode bln)
{
    SDL_SetTextureBlendMode(m_tex,bln);
}

void Sprite::GetSize(int& w,int& h)
{
    SDL_QueryTexture(m_tex,nullptr,nullptr,&w,&h);
}

void Sprite::GetPos(int& x,int& y)
{
    x = m_dst.x;
    y = m_dst.y;
}

void Sprite::SetRollAngle(double e)
{
    m_roll_ang = e;
}

void Sprite::SetRollCenter(int x,int y)
{
    m_roll_pnt.x = x;
    m_roll_pnt.y = y;
}

void Sprite::SetRollFlip(SDL_RendererFlip flip)
{
    m_roll_flp = flip;
}

void Sprite::SetRollEnable(bool b)
{
    m_roll = b;
}

Sprite::~Sprite()
{
    Clear();
    //PNT("Sprite Killed:"<<this<<std::endl;)
}

void Sprite::Load(Surface& s)
{
    Load((SDL_Surface*)s);
}

void Sprite::SetZoom(float f)
{
    int w,h;
    GetSize(w,h);
    SetZoom(int(w*f),int(h*f));
}

void Sprite::SetZoom(float w, float h)
{
    int wk,hk;
    GetSize(wk,hk);
    SetZoom(int(w*wk),int(h*hk));
}

void Sprite::Update(const SDL_Rect& area,const void* pixels,int pitch)
{
    SDL_UpdateTexture(m_tex,&area,pixels,pitch);
}

void Sprite::Update(const void* pixels,int pitch)
{
    SDL_UpdateTexture(m_tex,&m_src,pixels,pitch);
}


void* Sprite::Lock(const SDL_Rect& area,int& pitch)
{
    void* ret;
    SDL_LockTexture(m_tex,&area,&ret,&pitch);
    return ret;
}

void Sprite::Unlock()
{
    SDL_UnlockTexture(m_tex);
}

