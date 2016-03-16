#include <Windows.h>
#include <iphlpapi.h>
#include <cstdlib>
#include <SDL_net.h>
#include <Snow/Debug.h>

typedef DWORD (__stdcall* PGAINFO)(PIP_ADAPTER_INFO, PULONG);



static Uint64 getMac(){
    IP_ADAPTER_INFO adapter[5];
    DWORD buflen=sizeof(adapter);
    HMODULE hInst = LoadLibrary("iphlpapi.dll");
    PGAINFO pGAInfo = (PGAINFO)GetProcAddress(hInst, "GetAdaptersInfo");

    Uint64 mac = 0;
    DWORD status=pGAInfo(adapter,&buflen);
    if(status == ERROR_SUCCESS)
    {
        PIP_ADAPTER_INFO painfo=adapter;
        memcpy(&mac,painfo->Address,6);
        return mac;
    }else exit(-1);
}



struct UpLoad{
    Uint8 work;
    Uint8 keySeed;
    Uint64 mac;
};

struct DownLoad{
    Uint8 work;
    Uint8 key;	//pass ^keySeed
};

Uint8 BetaStart(){
    //Init
    UDPsocket udp;
    UDPpacket* pkg;
    udp = SDLNet_UDP_Open(0);
    pkg = SDLNet_AllocPacket(sizeof(UpLoad));
    SDLNet_ResolveHost(&pkg->address,"119.29.96.52",17768);
    pkg ->len = sizeof(UpLoad);

    //Login
    ((UpLoad*)pkg ->data) ->work = 1;
    srand(time(NULL));
    ((UpLoad*)pkg ->data) ->keySeed = rand()%256;
    ((UpLoad*)pkg ->data) -> mac = getMac();
    SDLNet_UDP_Send(udp,-1,pkg);
    PNT("Login....");
    while(1){
        if(SDLNet_UDP_Recv(udp,pkg)){
            if(((DownLoad*)pkg->data) -> work == 1){
                PNT("Login OK!");
                return ((DownLoad*)pkg->data) -> key;
            }
        }
    }
}


