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
        SDL_Texture* tex[5];
        int inii;
    }sc;
    data.ResetPtr();
    data.Read<ScbgData11>(sc);
    if(cnt == -1){
        sc.inii = 1211;
        for(int i = 1;i <= 5;++i){
            sc.tex[i-1] = LoadPic("Stage/Stage1/Daiyousei/SCBG/"+std::to_string(i)+".png");
        }
    }
    else if(cnt == -2){
        for(int i = 0;i < 5;++i){
            SDL_DestroyTexture(sc.tex[i]);
        }
    }
    else{
        for(int i = 0;i < 5;++i){
            SDL_RenderCopy(Snow::pRnd,sc.tex[i],nullptr,nullptr);
        }
    }
    data.ResetPtr();
    data.Write<ScbgData11>(sc);
}

void InitScBgs(){
    scbgs[11] = &SCBG11;
}
