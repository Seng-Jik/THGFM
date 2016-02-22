#include <list>
#include <map>
#include <atomic>
#include <set>
#include <mutex>
#include <thread>
#include <windows.h>
#define WITHOUT_SDL
#include <SDL_net.h>



//��������
struct Rank{
	char playerName[4];	//�����
	uint32_t score;	//����
};

std::atomic<int> fwNum;	//������
std::mutex rankLock;	//����������
std::mutex fileLock;	//�ļ���
std::mutex debug;


inline operator < (const Rank& r1,const Rank& r2){
	return r1.score > r2.score;
}

std::map<Uint8,std::multiset<Rank> > gameRank;

void Flush(Uint8 gameID){	//���ڴ��е��������µ��ļ�
    debug.lock();
    printf("Flush %d\n",gameID);
    debug.unlock();
	char fileName [16];
	sprintf(fileName,"game%d.txt",int(gameID));
	fileLock.lock();
	FILE* f = fopen(fileName,"w");

	std::multiset<Rank>* rank = &gameRank[gameID];
	int i = 0;
	rankLock.lock();
	for(auto p = rank -> begin();p != rank -> end() && i < 100;++p,++i)
		fprintf(f,"%c%c%c %d\n",p->playerName[0],p->playerName[1],p->playerName[2],p->score);
	fprintf(f,"||| 0");	//������־
	rankLock.unlock();
	fclose(f);
	fileLock.unlock();
}

bool LoadRank(Uint8 gameID){	//���ļ���ȡ��������,���سɹ���ʧ��
    debug.lock();
    printf("Loading %d\n",gameID);
    debug.unlock();
	char fileName [16];
	sprintf(fileName,"game%d.txt",int(gameID));
	fileLock.lock();
	FILE* f = fopen(fileName,"r");
	if(!f) return false;

	std::multiset<Rank> *rank = &gameRank[gameID];
	rankLock.lock();
	rank->clear();
	for(int i = 0;i < 100;++i){
		Rank r;
		int k;
        debug.lock();
        fscanf(f,"%c%c%c %d\n",&r.playerName[0],&r.playerName[1],&r.playerName[2],&k);
        printf("Rank %c%c%c %d\n",r.playerName[0],r.playerName[1],r.playerName[2],k);
        debug.unlock();
		r.score = i;
		r.playerName[3] = '\0';
		rank -> insert(r);
		if(r.playerName[0] == '|') break;
	}
	rankLock.unlock();
	fclose(f);
	fileLock.unlock();
	return true;
}

int WaitForData(TCPsocket tcp,void* data,int max=16){	//�ȴ��ͻ��˷������ݣ������ʱ�򷵻�0�����򷵻����ݳ���
	uint64_t beg = clock();
	while(1){
		int size = SDLNet_TCP_Recv(tcp,data,max);
		if(size > 0) return size;
		else
			if((beg - clock())/CLOCKS_PER_SEC > 5) return 0;
	}
}

bool SendRank(TCPsocket tcp,const std::multiset<Rank>* r){	//����������Ŀ��ͻ��ˣ����سɹ���ʧ��
	Uint8 buf[1+sizeof(Rank)*100];	//Ҫ���͵�����
	rankLock.lock();
	buf[0] = r->size();
	if(buf[0]>100) buf[0] = 100;

	int i = 0;
	for(auto p = r -> begin();p != r -> end() && i < 100;++p,++i)
		memcpy(&buf[1+sizeof(Rank)*i],&*p,sizeof(Rank));	//Todo:dst or src?
	rankLock.unlock();
	if(SDLNet_TCP_Send(tcp,buf,sizeof(buf)) > 0) return true;
	else return false;
}

void UpdateRank(std::multiset<Rank>* gameRank,Rank* r){	//���������ݲ�����������,�������µ��ļ�
	rankLock.lock();
	gameRank -> insert(*r);
	while(gameRank -> size() > 100){
		auto p = gameRank->end();
		p--;
		gameRank->erase(p);
	}
	rankLock.unlock();
	debug.lock();
	printf("Rank Updated:%c%c%c %d\n",r->playerName[0],r->playerName[1],r->playerName[2],r->score);
	debug.unlock();
}

bool SendLowestScore(TCPsocket tcp,std::multiset<Rank>* gameRank){	//������ͷֺ������������ͻ���,���سɹ���ʧ��
	Uint32 buf[2];
	rankLock.lock();

	auto p = gameRank->end();
	p--;
	buf[0] = p->score;
	buf[1] = gameRank->size();

	rankLock.unlock();
	if(SDLNet_TCP_Send(tcp,buf,sizeof(buf)) > 0) return true;
	else return false;
}

void ProcPlayer(TCPsocket tcp){	//���������
	uint8_t gameID;
	uint8_t buf[16];

	if(!WaitForData(tcp,buf)){SDLNet_TCP_Close(tcp);return;}
	if(buf[0] == 0x00) gameID = buf[1];
	else {SDLNet_TCP_Close(tcp);return;}

	fwNum++;
	while(WaitForData(tcp,buf)){
		switch(buf[0]){
		default:
		case 0xFF:
			SDLNet_TCP_Close(tcp);
			fwNum--;
			return;
		case 0x11:
			if(!SendRank(tcp,&gameRank[gameID])){SDLNet_TCP_Close(tcp);fwNum--;return;}
			break;
		case 0x12:
			UpdateRank(&gameRank[gameID],(Rank*)(&buf[1]));
			Flush(gameID);
			break;
		case 0x13:
			if(!SendLowestScore(tcp,&gameRank[gameID])){SDLNet_TCP_Close(tcp);fwNum--;return;}
			break;
		}
	}
	fwNum--;
	SDLNet_TCP_Close(tcp);
}

int main(){
	//����ʱ������������
	for(int i = 0;;++i)
		if(!LoadRank(i)) break;

	SDLNet_Init();
	//��������
	IPaddress ip;
	SDLNet_ResolveHost(&ip,nullptr,3098);
	TCPsocket server = SDLNet_TCP_Open(&ip);

	//��ʼ����
    debug.lock();
    printf("Listening...\n");
    debug.unlock();
	while(1){
		auto client = SDLNet_TCP_Accept(server);
		if(client){	//���̴߳���
            debug.lock();
            int k = fwNum;
            debug.lock();
            printf("A Player Linked,Now is %d players.\n",k);
            debug.unlock();
			while(fwNum > 200)	//��󲢷�����200
				Sleep(20);
			std::thread thd(ProcPlayer,client);
			thd.detach();
		}
		else Sleep(50);
	}
	return 0;
}

/** ����Э�� *

�ͻ��˶˷��͵����ݣ�
0x00	�������Ӻ󣬺��GameID
0x11	���󷵻�������
0x12	�����ϴ������������3��charΪ��������ٺ��uint32_tΪ����
0x13	������ͷֺ���������
0xFF	ֹͣ����

���������͵����ݣ�
	�������ݣ�
		Uint8 num;	//��������
		struct Rank[100];	//ǰ100������
**/
