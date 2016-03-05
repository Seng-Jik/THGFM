#include "RankClient.h"
#include <cstdio>
#include <iostream>
#define WITHOUT_SDL
#include <SDL_net.h>
using namespace std;
int main(){
    SDLNet_Init();
    Rank rs;
    rs.name[0] = 'L';
    rs.name[1] = 'M';
    rs.name[2] = 'A';
    rs.name[3] = '\0';
    rs.score = 99999;
    UpdateRank(1,rs);
    Rank r[100];
    if(GetRank(1,r))
        for(int i = 0;i < 100;++i){
            printf("%c%c%c %d\n",r[i].name[0],r[i].name[1],r[i].name[2],r[i].score);
        }
    else printf("Failed.\n");
    return 0;
}
