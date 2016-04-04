#ifndef STAGEMGR_H
#define STAGEMGR_H
#include <string>
#include <vector>
#include <queue>
#include <forward_list>
#include "Boss.h"
#include "Tools.h"
#include "Snow.h"

struct Shot{
    bool deadDanmaku = false;   //�Ƿ�ΪDָ������������
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
    int texNum; //��ǰʹ�õ�ͼ����
    SDL_RendererFlip flipMode;  //����ģʽ���Զ��ж�����ǰ��������ƶ����Զ���ת
    enum{NOBIRTH,LIVE,STOPLIVE,DEATH}live = NOBIRTH;
    double angle;   //�Ƕ�
    double x,y,spd; //x,y,�ٶ�,�ж��뾶
    int partten; //����ģʽ
    double hp;
    std::vector<Shot*> shots;    //���
    Shot* whenKilled =nullptr;   //���������
    int style;  //����ͼ����
    double parttenArgs[8];

    int items[2];   //�����ĵ���
};

class StageMgr
{
    private:
        static int m_cnt;
        std::vector<Enemy*> m_enemys;
        int m_enemySearchTop = 0;
        int m_enemySearchBottom = 0;

        struct EnemyStyle{
            SDL_Texture* tex[16];
            int texCount = 1;
            double r = 16;
            int frameJg;    //֡���
            bool autoFlip;  //�Զ�ת��
        };
        static EnemyStyle m_eStyles [128];

        std::queue<int> m_clearScreenTime;

        std::queue<Boss*> m_bosses;
        Level m_lv;

    public:
        static void Init();
        static inline int GetCnt(){return m_cnt;}

        StageMgr();
        void LoadCSV(const std::string&,const std::string& basePath,Level lv);
        Level GetLv(){return m_lv;}
        void Clear();
        void OnNext();
        void OnDraw();
        void KillEnemy(Enemy*);
        inline void KillEnemy(int num,double hp){
            m_enemys[num]->hp -= hp;
        }

        inline Enemy* GetEnemy(int n){return m_enemys[n];}
        Enemy* GetEnemy();  //���һ������
        inline Boss* GetBoss(){
            if(m_bosses.empty()) return nullptr;
            else if(m_bosses.front()->Live() && m_bosses.front()->birthTime <= m_cnt) return m_bosses.front();
            else return nullptr;
        }

        ~StageMgr();
    protected:
};

extern StageMgr stage;

#endif // STAGEMGR_H
