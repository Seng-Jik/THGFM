#ifndef STAGEMGR_H
#define STAGEMGR_H
#include <string>
#include <vector>
#include <forward_list>
#include "Snow.h"

struct Shot{
    int cnt;
    int birth; //出生时间
    enum{NOBIRTH,LIVE,STOPSHOOT,DEATH}live = NOBIRTH;
    //未出生,活着,停止射击,死亡
    int partten;    //射击模式
    std::vector<int> bullets;   //子弹集
    std::vector<double> parttenArgs;
};
struct Enemy{
    int cnt;
    int birth; //出生时间
    int num;
    enum{NOBIRTH,LIVE,STOPLIVE,DEATH}live = NOBIRTH;
    double angle;   //角度
    double x,y,spd; //x,y,速度,判定半径
    int partten; //敌人模式
    double hp;
    std::vector<Shot*> shots;    //射击
    int style;  //敌人图像编号
    std::vector<double> parttenArgs;
};

class StageMgr
{
    public:
        static void Init();

        StageMgr();
        void LoadCSV(const std::string&);
        void Clear();
        void OnNext();
        void OnDraw();
        void KillEnemy(Enemy*);

        Enemy* GetEnemy(int n){return m_enemys[n];}

        ~StageMgr();
    protected:
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
};

extern StageMgr stage;

#endif // STAGEMGR_H
