#ifndef BEATER_H
#define BEATER_H
#include <queue>
#include <string>

class Beater
{
    public:
        void SetCntPtr(int* mainCnt);

        void OnNext();
        void SetBeater(float bpm);
        void ChangeBpm(float bpm);
        //两者的不同：
        //前者会清空计时器，即为变换关卡时使用
        //后者不会，只会更改节奏，为进入Boss时使用
    protected:
    private:
        float m_cnt;
        //std::queue<int> m_beats;
        float m_idenFps;
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
