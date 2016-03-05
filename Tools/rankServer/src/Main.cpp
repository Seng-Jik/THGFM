#include "FileParser.h"
#include <stdio.h>
#define WITHOUT_SDL
#include <SDL_net.h>
#include <cstdlib>
#include <windows.h>

void ProcPacket(UDPpacket* pk,UDPsocket udp);

int main(){
	SDLNet_Init();
	auto pkg = SDLNet_AllocPacket(100*sizeof(Rank));
    auto sock = SDLNet_UDP_Open(17530);
    while(1){
        while(SDLNet_UDP_Recv(sock,pkg))
            ProcPacket(pkg,sock);
        Sleep(50);
    }
	return 0;
}
