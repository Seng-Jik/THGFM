#include "Snow.h"
#include "Snow/Debug.h"
using namespace Snow;
typedef void(*SCBg)(int cnt,Bundle<256>&);

/* �ر��cnt == -1ʱҪ���ʼ����
    cnt == -2ʱҪ�����ڻ���
*/
void SCBG001(int cnt,Bundle<256>&){
}

SCBg scbgs [80] = {
    &SCBG001
};
