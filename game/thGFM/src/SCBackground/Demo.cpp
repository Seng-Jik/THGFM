#include "Snow.h"
#include "Snow/Debug.h"
using namespace Snow;
typedef void(*SCBg)(int cnt,Bundle<256>&);

/* 特别的cnt == -1时要求初始化，
    cnt == -2时要求终期化。
*/
void SCBG001(int cnt,Bundle<256>&){
}

SCBg scbgs [80] = {
    &SCBG001
};
