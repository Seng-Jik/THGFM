#include <cstdio>
typedef unsigned int UINT32;
struct Rank{
	char name[4];
	UINT32 score;
};

int main(){
	Rank r;
	r.name[0] = '_';
	r.name[1] = '_';
	r.name[2] = '_';
	r.name[3] = '\0';
	r.score = 0;
	FILE* f = fopen("thgfmRank.dat","wb");
	for(int i = 0;i < 100*11;++i)
		fwrite(&r,sizeof(Rank),1,f);
	fclose(f);
	return 0;
}
