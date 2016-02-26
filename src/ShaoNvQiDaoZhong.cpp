#include "ShaoNvQiDaoZhong.h"
#include "Tools.h"
using namespace std;

SDL_Texture* ShaoNvQiDaoZhong::m_ani[89];

void ShaoNvQiDaoZhong::Init()
{
    string image;
    for(int i = BEG_BEG;i <= END_END;++i){
        image = "GameUI/Loading/snqdz00";
        if(i<=9) image += '0';
        image += to_string(i);
        image += ".png";
        m_ani[i] = LoadPic(image.c_str());
    }
}


ShaoNvQiDaoZhong::ShaoNvQiDaoZhong(void(*func)(Snow::THREAD_ID),Snow::Activity*)
{
    //ctor
}

ShaoNvQiDaoZhong::~ShaoNvQiDaoZhong()
{
    //dtor
}

void ShaoNvQiDaoZhong::OnShow()
{

}

void ShaoNvQiDaoZhong::OnHide()
{
    delete this;
}

void ShaoNvQiDaoZhong::OnDraw()
{

}

void ShaoNvQiDaoZhong::OnNext()
{

}
