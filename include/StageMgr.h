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
    bool deadDanmaku = false;   //是否为D指令造出的射击集
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
    int texNum; //当前使用的图像编号
    SDL_RendererFlip flipMode;  //翻脸模式，自动判断是向前还是向后移动并自动翻转
    enum{NOBIRTH,LIVE,STOPLIVE,DEATH}live = NOBIRTH;
    double angle;   //角度
    double x,y,spd; //x,y,速度,判定半径
    int partten; //敌人模式
    double hp;
    std::vector<Shot*> shots;    //射击
    Shot* whenKilled =nullptr;   //死亡后射击
    int style;  //敌人图像编号
    double parttenArgs[8];

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
            SDL_Texture* tex[16];
            int texCount = 1;
            double r = 16;
            int frameJg;    //帧间隔
            bool autoFlip;  //自动转脸
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
        Enemy* GetEnemy();  //获得一个敌人
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
