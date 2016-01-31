<<<<<<< HEAD
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
    int birth; //出生时间
    enum{NOBIRTH,LIVE,STOPLIVE,STOPSHOOT,DEATH}live = NOBIRTH;
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

    int items[2];   //爆出的道具
};

class StageMgr
{
    private:
        static int m_cnt;
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
        void LoadCSV(const std::string&,const std::string& basePath);
        void Clear();
        void OnNext();
        void OnDraw();
        void KillEnemy(Enemy*);
        inline void KillEnemy(int num,double hp){
            m_enemys[num]->hp -= hp;
        }

        inline Enemy* GetEnemy(int n){return m_enemys[n];}
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
=======
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
    int birth; //出生时间
    enum{NOBIRTH,LIVE,STOPLIVE,STOPSHOOT,DEATH}live = NOBIRTH;
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

    int items[2];   //爆出的道具
};

class StageMgr
{
    private:
        static int m_cnt;
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
        void LoadCSV(const std::string&,const std::string& basePath);
        void Clear();
        void OnNext();
        void OnDraw();
        void KillEnemy(Enemy*);
        inline void KillEnemy(int num,double hp){
            m_enemys[num]->hp -= hp;
        }

        inline Enemy* GetEnemy(int n){return m_enemys[n];}
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
>>>>>>> 720c0b59a97be74f02a7c85da646f3f62bece3a5
