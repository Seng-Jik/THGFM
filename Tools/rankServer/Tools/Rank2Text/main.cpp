#include <cstdio>
typedef unsigned int UINT32;
typedef unsigned char UINT8;
struct Rank{
	char name[3];
	UINT8 place;
	UINT32 score;
};

int main(){
	Rank r;
	FILE* f = fopen("thgfmRank.dat","rb");
	FILE* txt = fopen("thgfmRank.txt","w");

	for(int i = 0;i < 11;++i){
		fprintf(txt,"------- Rank ID:%d --------\n",i);
		for(int j = 0;j < 100;++j){
			fread(&r,sizeof(Rank),1,f);
			fprintf(txt,"%c%c%c %d Place%d\n",r.name[0],r.name[1],r.name[2],r.score,r.place);
		}
	}
	fclose(f);
	fclose(txt);
	return 0;
}
