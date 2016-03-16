#include "Beater.h"
#include "Snow.h"
#include "Snow/Debug.h"
using namespace Snow;

Beater beater;
void Beater::SetCntPtr(int* mainCnt)
{
}

void Beater::OnNext()
{
    m_cnt += 1;
    m_isBeat = false;
    if(m_cnt >= m_idenFps){
        m_isBeat = true;
        m_cnt -= m_idenFps;
        ++m_beatNum;
        if(m_beatNum >= 4) m_beatNum=0;
    }
    PNT("Beater 0  "<<m_cnt<<" "<<m_idenFps<<" "<<m_beatNum<<" "<<m_isBeat);
    //PNT("BEATER:"<<*m_pCnt<<" "<<(int)m_nextFps);
}

/*void Beater::LoadBeats(const std::string& f)
{
    Clear();
    Uint32 pos = 0;
    ResFile r(f);
    std::string s;
    while(pos < r.Size()){
        s = GetLine(r,pos);
        if(!s.empty()){
            int sn = atoi(s.c_str());
            m_beats.push(sn);
        }
    }
}*/
void Beater::SetBeater(double bpm)
{
    ChangeBpm(bpm);
}

void Beater::ChangeBpm(double bpm)
{
    m_cnt = 0;
    m_idenFps = 60/(bpm/60);
    m_beatNum = 0;
    m_isBeat = false;
}


/*void Beater::Clear()
{
    while(!m_beats.empty()) m_beats.pop();
    m_isBeat = false;
    m_beatNum = 0;
}
*/
