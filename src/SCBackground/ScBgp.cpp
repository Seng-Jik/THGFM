#include "Snow.h"
#include "Snow/Debug.h"
#include "Tools.h"
using namespace Snow;
typedef void(*SCBg)(int cnt,Bundle<256>&);
extern SCBg scbgs [80];

/* 特别的cnt == -1时要求初始化，
    cnt == -2时要求终期化。
*/
void SCBG11(int cnt,Bundle<256>& data){
    struct ScbgData11{
        SDL_Texture* tex[4];
        SDL_Rect src[4];
        float speed2,aspd2,x2;
    }sc;
    data.ResetPtr();
    data.Read<ScbgData11>(sc);
    if(cnt == -1){
        sc.speed2 = 0;
        sc.aspd2 = 0.02;
        for(int i = 1;i <= 4;++i){
            sc.tex[i-1] = LoadPic("Stage/Stage1/Daiyousei/SCBG/"+std::to_string(i)+".png");
            sc.src[i-1].x=0;sc.src[i-1].y = (1200-720)/2;sc.src[i-1].w = 1280;sc.src[i-1].h = 720;
        }
        sc.x2 = 0;
    }
    else if(cnt == -2){
        for(int i = 0;i < 4;++i){
            SDL_DestroyTexture(sc.tex[i]);
        }
    }
    else{
        //3.png
        if(sc.speed2 >= 5) sc.aspd2 = -0.02;
        else if(sc.speed2 <= -5) sc.aspd2 = 0.02;
        sc.speed2 += sc.aspd2;
        sc.x2+=sc.speed2;
        sc.src[2].x = sc.x2;

        //4.png
        sc.src[3].y++;
        if(sc.src[3].y == 1200) sc.src[3].y = 0;

        for(int i = 0;i < 4;++i){
            SDL_RenderCopy(Snow::pRnd,sc.tex[i],&sc.src[i],nullptr);
        }
    }
    data.ResetPtr();
    data.Write<ScbgData11>(sc);
}

void InitScBgs(){
    scbgs[11] = &SCBG11;
}
