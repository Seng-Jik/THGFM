#include <SDL_net.h>
#include "Types.h"
#include "FileParser.h"
#include <cstdio>

static void _ProcAllRank(UDPpacket* pk,UDPsocket udp){
    int rankID = pk->data[0]-0x80;
    if(rankID >= 11) return;
    pk -> len = sizeof(Rank)*100;
    ReadAllRank(rankID,(Rank*)pk -> data);
    printf("Send rank #%d to a player...",rankID);
    SDLNet_UDP_Send(udp,-1,pk);
    printf("OK.\n");
}
static void _ProcUpdate(UDPpacket* pk){
    if(pk->data[0] >= 11) return;
    Uint8 gameID = pk->data[0];
    Rank* r = (Rank*)&pk->data[1];
    UpdateRank(gameID,*r);
    printf("Player %c%c%c is updating rank #%d use %d score.\n",r->name[0],r->name[1],r->name[2],(int)pk->data[0],r->score);
}

void ProcPacket(UDPpacket* pk,UDPsocket udp){
    if(pk->data[0]>=0x80){
        //请求号0x80+i，请求第i个排名表的全排名
        _ProcAllRank(pk,udp);
    }
    else{
        //请求号i，请求为第i号排名表上传排名
        //从第1字节到第9字节为struct Rank内容
        _ProcUpdate(pk);
    }
}
