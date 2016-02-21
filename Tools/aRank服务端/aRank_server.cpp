#include <list>
#include <map>
#include <atomic>
#include <set>
#include <mutex>
#include <thread>
#include <windows.h>
#define WITHOUT_SDL
#include <SDL_net.h>



//排名对象
struct Rank{
	char playerName[4];	//玩家名
	uint32_t score;	//分数
};

std::atomic<int> fwNum;	//访问数
std::mutex rankLock;	//排名数据锁
std::mutex fileLock;	//文件锁
std::mutex debug;


inline operator < (const Rank& r1,const Rank& r2){
	return r1.score > r2.score;
}

std::map<Uint8,std::multiset<Rank> > gameRank;

void Flush(Uint8 gameID){	//把内存中的排名更新到文件
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
	fprintf(f,"||| 0");	//结束标志
	rankLock.unlock();
	fclose(f);
	fileLock.unlock();
}

bool LoadRank(Uint8 gameID){	//从文件读取排名数据,返回成功或失败
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

int WaitForData(TCPsocket tcp,void* data,int max=16){	//等待客户端返回数据，如果超时则返回0，否则返回数据长度
	uint64_t beg = clock();
	while(1){
		int size = SDLNet_TCP_Recv(tcp,data,max);
		if(size > 0) return size;
		else
			if((beg - clock())/CLOCKS_PER_SEC > 5) return 0;
	}
}

bool SendRank(TCPsocket tcp,const std::multiset<Rank>* r){	//发送排名表到目标客户端，返回成功或失败
	Uint8 buf[1+sizeof(Rank)*100];	//要发送的数据
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

void UpdateRank(std::multiset<Rank>* gameRank,Rank* r){	//把排名数据并放入排名表,但不更新到文件
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

bool SendLowestScore(TCPsocket tcp,std::multiset<Rank>* gameRank){	//发送最低分和排名个数到客户端,返回成功或失败
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

void ProcPlayer(TCPsocket tcp){	//处理单个玩家
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
	//启动时加载排名数据
	for(int i = 0;;++i)
		if(!LoadRank(i)) break;

	SDLNet_Init();
	//启动服务
	IPaddress ip;
	SDLNet_ResolveHost(&ip,nullptr,3098);
	TCPsocket server = SDLNet_TCP_Open(&ip);

	//开始监听
    debug.lock();
    printf("Listening...\n");
    debug.unlock();
	while(1){
		auto client = SDLNet_TCP_Accept(server);
		if(client){	//多线程处理
            debug.lock();
            int k = fwNum;
            debug.lock();
            printf("A Player Linked,Now is %d players.\n",k);
            debug.unlock();
			while(fwNum > 200)	//最大并发访问200
				Sleep(20);
			std::thread thd(ProcPlayer,client);
			thd.detach();
		}
		else Sleep(50);
	}
	return 0;
}

/** 数据协议 *

客户端端发送的数据：
0x00	建立连接后，后跟GameID
0x11	请求返回排名表
0x12	请求上传新排名，后跟3个char为玩家名，再后跟uint32_t为分数
0x13	请求最低分和排名个数
0xFF	停止连接

服务器发送的数据：
	排名数据：
		Uint8 num;	//排名个数
		struct Rank[100];	//前100名排名
**/
