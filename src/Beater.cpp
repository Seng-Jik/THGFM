#include "Beater.h"
#include "Snow.h"
using namespace Snow;

Beater beater;
void Beater::SetCntPtr(int* mainCnt)
{
    m_pCnt = mainCnt;
}

void Beater::OnNext()
{
    m_isBeat = false;
    if(int(m_nextFps) == *m_pCnt){
        m_isBeat = true;
        m_nextFps += m_idenFps;
        ++m_beatNum;
    }
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
    m_nextFps = m_idenFps = 60/(bpm/60);
    m_beatNum = 0;
    m_isBeat = false;
}

void Beater::ChangeBpm(double bpm)
{
    m_nextFps -= m_idenFps;
    m_idenFps = 60/(bpm/60);
    m_nextFps += m_idenFps;
    m_beatNum = 0;
}


/*void Beater::Clear()
{
    while(!m_beats.empty()) m_beats.pop();
    m_isBeat = false;
    m_beatNum = 0;
}
*/
