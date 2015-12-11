#ifndef STAGEMGR_H
#define STAGEMGR_H
#include <string>
#include <vector>
#include <queue>
#include <forward_list>
#include "Boss.h"
#include "Snow.h"

struct Shot{
    int cnt;
    int birth; //����ʱ��
    enum{NOBIRTH,LIVE,STOPLIVE,STOPSHOOT,DEATH}live = NOBIRTH;
    //δ����,����,ֹͣ���,����
    int partten;    //���ģʽ
    std::vector<int> bullets;   //�ӵ���
    std::vector<double> parttenArgs;
};
struct Enemy{
    int cnt;
    int birth; //����ʱ��
    int num;
    enum{NOBIRTH,LIVE,STOPLIVE,DEATH}live = NOBIRTH;
    double angle;   //�Ƕ�
    double x,y,spd; //x,y,�ٶ�,�ж��뾶
    int partten; //����ģʽ
    double hp;
    std::vector<Shot*> shots;    //���
    int style;  //����ͼ����
    std::vector<double> parttenArgs;
};

class StageMgr
{
    private:
        int m_cnt = 0;
        std::vector<Enemy*> m_enemys;
        int m_enemySearchTop = 0;
        int m_enemySearchBottom = 0;

        struct EnemyStyle{
            SDL_Texture* tex;
            int texCount = 1;
            double r = 16;
        };
        static EnemyStyle m_eStyles [1];

        std::queue<int> m_clearScreenTime;

        std::queue<Boss*> m_bosses;

    public:
        static void Init();

        StageMgr();
        void LoadCSV(const std::string&);
        void Clear();
        void OnNext();
        void OnDraw();
        void KillEnemy(Enemy*);
        inline void KillEnemy(int num,double hp){
            m_enemys[num]->hp -= hp;
        }

        Enemy* GetEnemy(int n){return m_enemys[n];}

        ~StageMgr();
    protected:
};

extern StageMgr stage;

#endif // STAGEMGR_H
