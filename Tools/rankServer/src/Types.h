#pragma once

typedef unsigned char UINT8;
typedef unsigned int UINT32;

struct Rank{
	char name[3];
	UINT8 place;
	/** Place List **
	* 00 China
	* 01 China Hongkong
	* 02 China Macau
	* 03 China TaiWan
	* 04 Japan
	* 05 America
	**/
	UINT32 score;
};

