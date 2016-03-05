#include "FileParser.h"
#include <cstdio>
#include <conio.h>
FILE* _file;

static bool _OpenFileRO(UINT8 rankID){
	_file = fopen("thgfmRank.dat","rb");
	fseek(_file,sizeof(Rank)*100*rankID,SEEK_SET);
	return _file != nullptr;
}

static bool _OpenFileRW(UINT8 rankID){
	_file = fopen("thgfmRank.dat","rb+");
	fseek(_file,sizeof(Rank)*100*rankID,SEEK_SET);
	return _file != nullptr;
}

static void _CloseFile(){
	fclose(_file);
}

void ReadAllRank(UINT8 rankID,Rank* r){
	if(!_OpenFileRO(rankID)){
		printf("ERROR:DONT OPEN RANK FILE!(IN FUNCTION READALLRANK)\n");
		getch();
		return;
	}else{
		fread(r,sizeof(Rank)*100,1,_file);
		_CloseFile();
	}
}

void UpdateRank(UINT8 rankID,const Rank& r){
	if(!_OpenFileRW(rankID)){
		printf("ERROR:DONT OPEN RANK FILE!(IN FUNCTION UPDATE RANK)\n");
		getch();
		return;
	}else{
		Rank mem[100];
		auto offset = ftell(_file);
		fread(mem,sizeof(Rank),100,_file);
		for(int i = 99;i >= -1;--i){
            if(i == -1){
                mem[0] = r;
                break;
            }else if(r.score <= mem[i].score){
                if(i != 99) mem[i+1] = r;
                break;
            }
            if(r.score > mem[i].score){
                if(i != 99) mem[i+1] = mem[i];
            }
		}
		fseek(_file,offset,SEEK_SET);
		fwrite(mem,sizeof(Rank),100,_file);
		_CloseFile();
	}
}

