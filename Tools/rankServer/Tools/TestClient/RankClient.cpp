#define WITHOUT_SDL
#include <SDL_net.h>
#include <mem.h>
#include "RankClient.h"
#include <ctime>

bool GetRank(UINT8 rankID,Rank* ranks){
    UDPpacket* pkg = SDLNet_AllocPacket(100*sizeof(Rank));
    pkg -> len = 1;
    pkg -> data[0] = rankID + 0x80;

    auto sock = SDLNet_UDP_Open(0);
    SDLNet_ResolveHost(&pkg->address,"119.29.96.52",17530);
    SDLNet_UDP_Send(sock,-1,pkg);
    bool ok = true;
    auto timebeg = clock();
    while(!SDLNet_UDP_Recv(sock,pkg)){
        if((clock() - timebeg)/CLOCKS_PER_SEC >= 5){
            ok = false;
            break;
        }
    }
    if(ok) memcpy(ranks,pkg->data,100*sizeof(Rank));
    SDLNet_UDP_Close(sock);
    SDLNet_FreePacket(pkg);
    return ok;
}

void UpdateRank(UINT8 rankID,const Rank& r){
    auto pkg = SDLNet_AllocPacket(16);
    pkg -> len = sizeof(Rank)+1;
    pkg -> data[0] = rankID;
    *((Rank*)&pkg->data[1]) = r;

    auto sock = SDLNet_UDP_Open(0);
    SDLNet_ResolveHost(&pkg->address,"119.29.96.52",17530);
    SDLNet_UDP_Send(sock,-1,pkg);
    SDLNet_UDP_Close(sock);
    SDLNet_FreePacket(pkg);
}
