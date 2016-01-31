<<<<<<< HEAD
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
    if(*m_pCnt == m_beats.front()){
        m_isBeat = true;
        ++m_beatNum;
        m_beats.pop();
    };
}

void Beater::LoadBeats(const std::string& f)
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
}

void Beater::Clear()
{
    while(!m_beats.empty()) m_beats.pop();
    m_isBeat = false;
    m_beatNum = 0;
}
=======
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
    if(*m_pCnt == m_beats.front()){
        m_isBeat = true;
        ++m_beatNum;
        m_beats.pop();
    };
}

void Beater::LoadBeats(const std::string& f)
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
}

void Beater::Clear()
{
    while(!m_beats.empty()) m_beats.pop();
    m_isBeat = false;
    m_beatNum = 0;
}
>>>>>>> 720c0b59a97be74f02a7c85da646f3f62bece3a5
