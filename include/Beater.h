#ifndef BEATER_H
#define BEATER_H
#include <queue>
#include <string>

class Beater
{
    public:
        void SetCntPtr(int* mainCnt);

        void OnNext();
        void LoadBeats(const std::string&);
        void Clear();
    protected:
    private:
        int* m_pCnt;
        std::queue<int> m_beats;
        bool m_isBeat;
        int m_beatNum;

    public:
        inline int GetBeatNum(){
            return m_beatNum;
        }
        inline bool IsBeatFrame(){
            return m_isBeat;
        }
};

extern Beater beater;

#endif // BEATER_H
