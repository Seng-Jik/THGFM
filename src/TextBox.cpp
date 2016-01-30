#include "TextBox.h"
#include "MathFunc.h"

using namespace ::Snow;
using namespace std;

namespace ACGCross {
namespace Galgame {

static ResVal r;

TextBox::TextBox()
{
    r.Load("TextBox.rv");
    m_fpsSpeed=m_fpsEffectSpeed=m_nowFps=255;
    m_fpsCounter = m_rect.x = m_rect.y = 0;
    m_rect.w = m_rect.h=500;
    m_linePos = m_heiPos = 0;
    m_rfont.Load("FangZhengHei.TTF");
    m_tfont = TTF_OpenFontRW(m_rfont,m_rfont.Size(),r.Int("GAL_WORDSIZE"));
    m_word_h = r.Int("GAL_WORDSIZE");
}

TextBox::~TextBox()
{
    //dtor
}

void TextBox::SetColor(int r, int g, int b)
{m_color.r = r;m_color.g = g;m_color.b = b;}

void TextBox::SetFontSize(int px){
    if(px > m_biggestLineHeight) m_biggestLineHeight = px;
    m_word_h = px;
}

void TextBox::Br()
{
    m_lineWord.push_back(0);
    m_linePos = 0;
    m_heiPos += m_biggestLineHeight;
    m_biggestLineHeight = m_word_h;
}

void TextBox::ForceClear()
{
    m_biggestLineHeight = m_word_h;
    m_tpiv.clear();
    FOR_EACH(p,m_text.begin(),m_text.end()) delete *p;
    m_text.clear();
    m_lineWord.clear();
    m_lineWord.push_back(0);
    m_stat = TXT_KILLING;
    m_linePos = 0;
    m_heiPos = 0;
    m_showing_word = 0;
    m_fpsCounter = m_nowFps;
}

void TextBox::Clear()
{
    m_stat = TXT_KILLING;
    m_fpsCounter = m_nowFps;
}

Uint32 TextBox::ForceAddText(const std::wstring& s)
{

    Uint32 pFirstTex = m_text.size();
    FOR_EACH(p,s.begin(),s.end())
    {
        //if(m_cfont.GetCharWei(*p) <= 0) continue;
        m_lineWord[m_lineWord.size()-1] ++;

        /*渲染文字纹理*/
        m_text.push_back(new ::Snow::Sprite());
        const auto pTex = m_text.size() - 1;


        auto pSur = TTF_RenderGlyph_Blended(m_tfont,*p,m_color);
        ::Snow::DrawTextOutLine(pSur);
        m_text[pTex]  -> Load(pSur);
        SDL_FreeSurface(pSur);


        /*求文字需要的数据*/
        int w,h;
        m_text[pTex] -> GetSize(w,h);

        //缩放
        {
            float per = float(m_word_h)/r.Int("GAL_WORDSIZE");
            m_text[pTex] -> SetZoom(per);
            w *= per;
        }

        //位置
        {
            if(w + m_linePos > m_rect.w)
                Br();

            m_text[pTex] ->SetPos(m_rect.x + m_linePos,m_rect.y + m_heiPos);
            m_linePos += w;
        }

    }
    return pFirstTex;
}


Uint32 TextBox::ForceAddPic(const std::string& file, const int fps, const int time)
{
    m_lineWord[m_lineWord.size()-1] ++;

    /*渲染纹理*/
    m_text.push_back(new ::Snow::Sprite());
    auto pTex = m_text.size() - 1;

    m_text[pTex] -> Load(file);
    int w,h;
    m_text[pTex] -> GetSize(w,h);
    TextPicInfo info;
    info.tex = pTex;
    info.fpsCounter = 0;
    info.nowFps = 1;
    info.fpsCount = fps;
    info.fpsTime = float(time) / 1000 * FPS;

    m_text[pTex]-> SetSrc(0,0,w/(info.fpsCount),h);
    m_text[pTex] -> SetZoom(w/(info.fpsCount),h);
    m_tpiv.push_back(info);

    //位置
    {
        int w,h;
        m_text[pTex] -> GetSize(w,h);
        if(w + m_linePos > m_rect.w)
            Br();

        m_text[pTex] ->SetPos(m_rect.x + m_linePos,m_rect.y + m_heiPos);
        m_linePos += w;
    }
    return pTex;
}




void TextBox::AddText(const std::wstring& s)
{
    auto pst = ForceAddText(s);
    FOR_EACH(p,pst,m_text.size())
        m_text[p] -> SetAlpha(0);
    if(m_stat != TXT_SHOWING) m_showing_word = pst;
    m_stat = TXT_SHOWING;
    m_fpsCounter = m_nowFps;

}

void TextBox::AddPic(const std::string& file,const int fps,const int time)
{
    auto pst = ForceAddPic(file,fps,time);
    m_text[pst] -> SetAlpha(0);
    if(m_stat != TXT_SHOWING) m_showing_word = pst;
    m_stat = TXT_SHOWING;
    m_fpsCounter = m_nowFps;
}

void TextBox::OnNext()
{
    FOR_EACH(p,m_tpiv.begin(),m_tpiv.end()) //文字框中的表情图片动画
    {
        p -> fpsCounter++;
        if(p -> fpsTime <= p -> fpsCounter)
        {
            int w,h;
            m_text[p -> tex] -> GetSize(w,h);
            p -> fpsCounter = 0;
            p -> nowFps++;
            if(p -> nowFps >= p -> fpsCount) p -> nowFps = 0;

            m_text[p -> tex] -> SetSrc(w/p -> fpsCount* p -> nowFps,0,w/p -> fpsCount,h);
        }
    }

    if(m_stat == TXT_SHOWING){  //正在显示文字
        m_nowFps++;
        if(m_nowFps - m_fpsCounter >= Uint16(m_fpsSpeed))   //如果已经结束
        {
            m_text[m_showing_word] -> SetAlpha(255);
            m_showing_word ++;
            if(m_showing_word == m_text.size()){
                m_stat = NOEVENT;
                return;
            }
            m_fpsCounter = m_nowFps;
        }else{
            m_text[m_showing_word] -> SetAlpha(
                ArcFunc(float(m_nowFps - m_fpsCounter) / m_fpsSpeed) * 255);
        }
    }else if(m_stat == TXT_KILLING){    //正在消除文字
        m_nowFps++;
        if(m_nowFps - m_fpsCounter >= Uint16(m_fpsEffectSpeed)){
            ForceClear();
            m_stat = NOEVENT;
            return;
        }else{
            FOR_EACH(p,m_text.begin(),m_text.end())
                (*p) -> SetAlpha(255 - ArcFunc(float(m_nowFps - m_fpsCounter) / m_fpsEffectSpeed) * 255);
        }
    }/*else if(m_stat == TXT_STOPSPEAK){  //强制结束对话
        m_nowFps++;
        if(m_nowFps - m_fpsCounter >= Uint16(m_fpsEffectSpeed)){
            m_stat = NOEVENT;
            FOR_EACH(ps,m_text.begin(),m_text.end())
                ps -> SetAlpha(255);
            return;
        }else{
            FOR_EACH(ps,m_showing_word,m_text.end())
                ps -> SetAlpha(ArcFunc(float(m_nowFps - m_fpsCounter) / m_fpsEffectSpeed) * 255);
        }
    }*/else if(m_stat == RECT_HIDING){    //正在隐藏文字框
        m_nowFps++;
        if(m_nowFps - m_fpsCounter >= Uint16(m_fpsEffectSpeed)){
            FOR_EACH(p,m_text.begin(),m_text.end())
                (*p) -> SetAlpha(0);
            m_stat = RECT_HIDE;
            return;
        }else{
            FOR_EACH(p,m_text.begin(),m_text.end())
                (*p) -> SetAlpha(255 - ArcFunc(float(m_nowFps - m_fpsCounter) / m_fpsEffectSpeed) * 255);
        }
    }else if(m_stat == RECT_SHOWING){    //正在隐藏文字框
        m_nowFps++;
        if(m_nowFps - m_fpsCounter >= Uint16(m_fpsEffectSpeed)){
            FOR_EACH(p,m_text.begin(),m_text.end())
                (*p) -> SetAlpha(255);
            m_stat = NOEVENT;
            return;
        }else{
            FOR_EACH(p,m_text.begin(),m_text.end())
                (*p) -> SetAlpha(ArcFunc(float(m_nowFps - m_fpsCounter) / m_fpsEffectSpeed) * 255);
        }
    }
}

void TextBox::OnDraw()
{
    if(m_stat != RECT_HIDE) FOR_EACH(p,m_text.begin(),m_text.end())
        (*p) -> OnDraw();
}

void TextBox::Init()
{
    m_lineWord.push_back(0);
    m_fpsSpeed = 20;
    m_stat = NOEVENT;
    m_nowFps = 0;
    SetColor(255,255,255);
    m_showing_word = 0;

    m_linePos = 0;  //现在已经显示到的横行坐标
    m_heiPos = 0;  //现在已经显示到的竖行坐标
}

void TextBox::Destroy()
{}

void TextBox::SetSpeed(int time)
{m_fpsSpeed = float(time) / 1000 * FPS;}

void TextBox::SetEffectSpeed(int time)
{m_fpsEffectSpeed = float(time) / 1000 * FPS;}

void TextBox::StopSpeak()
{
    //m_stat = TXT_STOPSPEAK;m_fpsCounter = m_nowFps;
    m_stat = NOEVENT;
    FOR_EACH(ps,m_text.begin(),m_text.end())
        (*ps) -> SetAlpha(255);
}

void TextBox::SetRect(int x, int y, int w, int h)
{m_rect = {x,y,w,h};}

void TextBox::Hide()
{m_stat = RECT_HIDING;m_fpsCounter = m_nowFps;}

void TextBox::Show()
{m_stat = RECT_SHOWING;m_fpsCounter = m_nowFps;}





} // namespace Galgame
} // namespace ACGCross
