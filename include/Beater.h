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
        //���ߵĲ�ͬ��
        //ǰ�߻���ռ�ʱ������Ϊ�任�ؿ�ʱʹ��
        //���߲��ᣬֻ����Ľ��࣬Ϊ����Bossʱʹ��
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
