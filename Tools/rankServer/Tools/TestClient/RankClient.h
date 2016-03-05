typedef unsigned char UINT8;
typedef unsigned int UINT32;

struct Rank{
	char name[3];
	UINT8 place;
	UINT32 score;
};

bool GetRank(UINT8 rankID,Rank* ranks);
void UpdateRank(UINT8 rankID,const Rank& r);
