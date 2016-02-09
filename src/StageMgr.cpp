#include "StageMgr.h"
#include "Snow.h"
#include "Tools.h"
#include <cmath>
#include "Snow/Debug.h"
#include "EnePartten.h"
#include "ShotParttenAPI.h"
#include "CollWorld.h"
#include "StageTitle.h"
#include "Boss.h"
#include "EffectMgr.h"
#include "Beater.h"
#include "SeMgr.h"
#include "ItemMgr.h"
#include "BossConversation.h"

using namespace Snow;

StageMgr stage;

StageMgr::EnemyStyle StageMgr::m_eStyles [1];
int StageMgr::m_cnt = 0;

void StageMgr::Init()
{
    m_eStyles[0].tex = LoadPic("Enemy/e0.png");
    //bossConversation->SetFPSCnt(&m_cnt);
}


StageMgr::StageMgr()
{
    beater.SetCntPtr(&m_cnt);
}

StageMgr::~StageMgr()
{
    //dtor
}

void StageMgr::LoadCSV(const std::string& stage,const std::string& basePath)
{
    CSVReader csv;
    csv.LoadCSV(stage);

    Enemy* last = nullptr;
    do{
        std::string firstPop;
        if(!csv.PopStr(firstPop)) continue;
        if(firstPop[0] == '#') continue;
        else if(firstPop[0] == 'E'){
            auto e = new Enemy;
            csv.PopInt(e -> birth);
            csv.PopFloat(e -> x);
            csv.PopFloat(e -> y);
            csv.PopFloat(e -> angle);
            e -> angle = Ang2Arc(e -> angle);
            csv.PopFloat(e -> spd);
            csv.PopInt(e -> partten);
            csv.PopFloat(e -> hp);
            csv.PopInt(e -> style);
            e -> num = m_enemys.size();

            csv.PopInt(e -> items[SCORE]);
            csv.PopInt(e -> items[POWER]);
            double args;
            while(csv.PopFloat(args))
                e->parttenArgs.push_back(args);

            m_enemys.push_back(e);
            last = e;
        }else if(firstPop[0] == 'S'){
            auto s = new Shot;
            csv.PopInt(s->birth);
            s->birth += last ->birth;
            csv.PopInt(s->partten);
            double args;
            while(csv.PopFloat(args))
                s->parttenArgs.push_back(args);
            last -> shots.push_back(s);
        }else if(firstPop[0] == 'C'){
            int i;
            csv.PopInt(i);
            m_clearScreenTime.push(i);
        }else if(firstPop[0] == 'B'){
            auto pBoss = new Boss;
            csv.PopInt(pBoss -> birthTime);
            string bossRV;
            csv.PopStr(bossRV);
            pBoss -> LoadRV(basePath + bossRV,basePath,&m_cnt);
            m_bosses.push(pBoss);
        }
    }while(csv.NextLine());

    collWorld.SetEnemyBuffurSize(m_enemys.size());
}

void StageMgr::Clear()
{
    FOR_EACH(p,m_enemys.begin(),m_enemys.end()){
        for(auto pShot = (*p) -> shots.begin();pShot != (*p)->shots.end();++pShot){
            delete *pShot;
        }
        delete *p;
    }
    m_enemys.clear();
    m_cnt = 0;
    m_enemySearchTop = 0;
    m_enemySearchBottom = 0;

    while(!m_clearScreenTime.empty()) m_clearScreenTime.pop();
    while(!m_bosses.empty()) {delete m_bosses.front();m_bosses.pop();}

    collWorld.ClearEnemy();

}

void StageMgr::OnNext()
{
    if(m_cnt >= stageTitle.GetBeginTime() && m_cnt <= stageTitle.GetEndTime())
        stageTitle.OnNext();


    //产生即将出生的敌人
    for(int i = m_enemySearchTop;i < (int)m_enemys.size();++i){
        if(m_cnt == m_enemys[i]->birth){
            m_enemySearchTop=i+1;
            if(m_enemySearchTop > (int)m_enemys.size()) m_enemySearchTop = m_enemys.size();
            m_enemys[i]->cnt = 0;
            m_enemys[i]->live = Enemy::LIVE;
        }else break;
    }
    //PNT(m_enemySearchBottom<<" "<<m_enemySearchTop);

    //C 指令处理
    if(!m_clearScreenTime.empty())
        if(m_cnt == m_clearScreenTime.front()){
            m_clearScreenTime.pop();
            for(int i = m_enemySearchBottom;i < m_enemySearchTop;++i){
                if(m_enemys[i] -> live == Enemy::LIVE){
                    effMgr.Install(0,m_enemys[i]->x,m_enemys[i]->y);
                    KillEnemy(m_enemys[i]);
                }
            }
            for(int i = 0;i < bulletMgr.GetSearchTop();++i){
                if(bulletMgr[i].live) effMgr.Install(0,bulletMgr[i].x,bulletMgr[i].y);
            }
            bulletMgr.Clear();
        }

    //Boss 处理
    if(!m_bosses.empty()){
        if(m_bosses.front() -> birthTime == m_cnt)
            m_bosses.front() -> OnBirth();
        if(m_cnt >= m_bosses.front() -> birthTime){
            m_bosses.front() -> OnNext();
            if(!m_bosses.front()->Live()){
                delete m_bosses.front();
                m_bosses.pop();
                PNT("BOSS KILLED");
            }
        }
    }

    int bestLeft = WIDTH,bestRight = 0;
    for(int enemyNum = m_enemySearchBottom;enemyNum < m_enemySearchTop;++enemyNum){
        Enemy& enemy = *m_enemys[enemyNum];
        if(enemy.live == Enemy::LIVE){

            //如果生命值为0则杀死
            if(enemy.hp <= 0){
                KillEnemy(&enemy);
                effMgr.Install(0,enemy.x,enemy.y);
                itemMgr.AddItem(SCORE,10,enemy.x,enemy.y,enemy.items[SCORE]);
                itemMgr.AddItem(POWER,15,enemy.x,enemy.y,enemy.items[POWER]);
                se.Play(DEMOSE);
            }

            //应用 Partten
            if(enemy.partten != -1)
                enemyPartten[enemy.partten](&enemy,enemyNum);
            ++enemy.cnt;

            //坐标运算
            enemy.y -= enemy.spd * sin(enemy.angle);
            enemy.x -= enemy.spd * cos(enemy.angle);

            //如果超出屏幕则杀死
            if((enemy.x < -2*m_eStyles[enemy.style].r ||
                enemy.x > WIDTH + m_eStyles[enemy.style].r||
                enemy.y > HEIGHT + m_eStyles[enemy.style].r||
                enemy.y< -2*m_eStyles[enemy.style].r)&&
               enemy.cnt >= 180){
                    KillEnemy(&enemy);
                    //PNT(i<<"Killed By Screen.");
            }else{
                //同步碰撞刚体
                collWorld.SetEnemy(enemy.num,enemy.live == Enemy::LIVE,enemy.x,enemy.y,m_eStyles[enemy.style].r);
                if(enemy.x - m_eStyles[enemy.style].r - 16 < bestLeft) bestLeft = enemy.x - m_eStyles[enemy.style].r - 16;
                if(enemy.x + m_eStyles[enemy.style].r + 16 > bestRight) bestRight = enemy.x + m_eStyles[enemy.style].r + 16;
            }


        }

        //射击处理
        bool allShotDied = true;
        if(enemy.live == Enemy::LIVE || enemy.live == Enemy::STOPLIVE){
            for(auto p = enemy.shots.begin();p != enemy.shots.end();++p){
                Shot& shot = **p;

                //产生即将出生的射击
                if(shot.live == Shot::NOBIRTH && shot.birth == m_cnt && enemy.live == Enemy::LIVE) {
                    shot.live = Shot::LIVE;
                    shot.cnt = 0;
                }
                //销毁已经空了的射击
                else if((shot.live == Shot::LIVE && shot.cnt >= 300) || shot.live == Shot::STOPLIVE || shot.live == Shot::STOPSHOOT){
                    //子弹槽是否已经空掉
                    bool noBullet = true;
                    for(auto pBullet = shot.bullets.begin();pBullet != shot.bullets.end();++pBullet){
                        if(*pBullet != -1){
                            noBullet = false;
                            break;
                        }
                    }
                    if(noBullet){
                        shot.live = Shot::DEATH;
                        shot.bullets.clear();
                        continue;
                    }
                }

                if(shot.live == Shot::LIVE || shot.live == Shot::STOPLIVE || shot.live == Shot::STOPSHOOT) allShotDied = false;

                if(shot.live == Shot::LIVE || shot.live == Shot::STOPSHOOT){
                    //应用 Partten
                    (*shotPartten[shot.partten])(&shot,enemy.num);
                    ++shot.cnt;
                }

            }

        }

        //当敌人射击全部死亡且敌人已经停止活动时，彻底销毁敌人
        if(allShotDied && enemy.live == Enemy::STOPLIVE){
            FOR_EACH(p,enemy.shots.begin(),enemy.shots.end())
                delete *p;
            enemy.live = Enemy::DEATH;
            enemy.shots.clear();
            //PNT(enemy.num<<" Was Died");
            //探索搜索下限（死亡上限）
            while(m_enemys[m_enemySearchBottom]->live == Enemy::DEATH){
                ++m_enemySearchBottom;
                if(m_enemySearchBottom > m_enemySearchTop -1) {
                    m_enemySearchBottom = m_enemySearchTop -1;
                    break;
                }
                //PNT(m_enemySearchBottom<<" "<<m_enemySearchTop);
            }
        }

    }

    collWorld.SetEnemyXRect(bestLeft,bestRight);
    collWorld.SetEnemySearchTop(m_enemySearchTop,m_enemySearchBottom);
    ++m_cnt;

    //logc<<"beg"<<endl;
    //logc.flush();
    /*for(int i = m_enemySearchBottom;i < m_enemySearchTop;++i){
        bool allShotDied = true;
        FOR_EACH(pShot,m_enemys[i]->shots.begin(),m_enemys[i]->shots.end()){
            Shot& shot = **pShot;
            if(shot.live == Shot::STOPLIVE){
                bool noBullet = true;
                for(int i = 0;i < (int)shot.bullets.size();++i){
                    if(bulletMgr[shot.bullets[i]].live == false) shot.bullets[i] = -1;
                    if(shot.bullets[i] != -1) {noBullet = false;break;}
                }
               if(noBullet) shot.live = Shot::DEATH;
               continue;
            }
            if(shot.live == Shot::DEATH){
                shot.bullets.clear();
                continue;
            }

            allShotDied = false;
            if(shot.live == Shot::NOBIRTH && shot.birth == m_cnt) {
                shot.live = Shot::LIVE;
                shot.cnt = 0;
            }
            if(shot.live == Shot::NOBIRTH) continue;
            //TODO:射击模式处理
            if((shot.live == Shot::LIVE || shot.live == Shot::STOPLIVE) && shot.partten != -1)
                (*shotPartten[shot.partten])(&shot,i);

            ++shot.cnt;
        }

        if(m_enemys[i]->live == Enemy::LIVE){
            //When HP == 0 Kill Enemy
            if(m_enemys[i] -> hp <= 0){
                effMgr.Install(0,m_enemys[i]->x,m_enemys[i]->y);
                KillEnemy(m_enemys[i]);
                //PNT(i<<"Killed By Player Bullet.");
                continue;
            }

            //TODO:敌人模式
            if(m_enemys[i] -> partten != -1)
                enemyPartten[m_enemys[i] -> partten](m_enemys[i],i);

            ++m_enemys[i] -> cnt;
            m_enemys[i] -> y -= m_enemys[i] ->spd * sin(m_enemys[i] -> angle);
            m_enemys[i] -> x -= m_enemys[i] ->spd * cos(m_enemys[i] -> angle);


            if((m_enemys[i] -> x < -2*m_eStyles[m_enemys[i] -> style].r ||
                m_enemys[i] -> x > WIDTH + m_eStyles[m_enemys[i] -> style].r||
                m_enemys[i] -> y > HEIGHT + m_eStyles[m_enemys[i] -> style].r||
                m_enemys[i] -> y < -2*m_eStyles[m_enemys[i] -> style].r)&&
               m_enemys[i] ->cnt >= 180
                ){
                    KillEnemy(m_enemys[i]);
                    //PNT(i<<"Killed By Screen.");
                }

            if(i == 0){
                PNT("allShotDied[0] "<<allShotDied<<"  Stat:"<<m_enemys[i] -> live);
            }
            if(allShotDied && m_enemys[i] -> live == Enemy::STOPLIVE){
                FOR_EACH(p,m_enemys[i]->shots.begin(),m_enemys[i]->shots.end())
                    delete *p;
                m_enemys[i] -> live = Enemy::DEATH;
                m_enemys[i]->shots.clear();
                PNT(i<<"Fully Killed.");
                for(int j = 0;j<m_enemys[0]->shots.size();++j){
                   //PNT(j<<" Shot "<<m_enemys[0]->shots[j]->live);
                   //system("pause");
                }
            }
        }
        collWorld.SetEnemy(i,m_enemys[i]->live == Enemy::LIVE,m_enemys[i]->x,m_enemys[i] ->y,m_eStyles[m_enemys[i]->style].r,m_enemySearchBottom,m_enemySearchTop);
    }
    //logc<<"end"<<endl;
    //logc.flush();
    collWorld.Update_Player_Enemy();


    //logc<<"coll end"<<endl;
    //logc.flush();*/
}

void StageMgr::OnDraw()
{
    for(int i = m_enemySearchBottom;i < m_enemySearchTop;++i){
        if(m_enemys[i]->live != Enemy::LIVE) continue;
        SDL_Rect r = {int(m_enemys[i]->x - m_eStyles[m_enemys[i]->style].r),
                             int(m_enemys[i]->y - m_eStyles[m_enemys[i]->style].r),
                             int(2*m_eStyles[m_enemys[i]->style].r),
                             int(2*m_eStyles[m_enemys[i]->style].r)
        };
        SDL_RenderCopy(pRnd,m_eStyles[m_enemys[i]->style].tex,nullptr,&r);
    }

    //Boss 处理
    if(!m_bosses.empty()){
        if(m_cnt >= m_bosses.front() -> birthTime){
            m_bosses.front() -> OnDraw();
        }
    }

    if(m_cnt >= stageTitle.GetBeginTime() && m_cnt <= stageTitle.GetEndTime())
        stageTitle.OnDraw();

    SDL_SetRenderDrawColor(Snow::pRnd,255,0,0,255);
    char frameStr [16];
    itoa(m_cnt,frameStr,10);
    SDLTest_DrawString(Snow::pRnd,WIDTH-200,HEIGHT-16,frameStr);
}

void StageMgr::KillEnemy(Enemy* e)
{
    if(e == nullptr){
        for(int i = m_enemySearchBottom;i < m_enemySearchTop;++i){
            if(m_enemys[i] -> live == Enemy::LIVE){
                effMgr.Install(0,m_enemys[i]->x,m_enemys[i]->y);
                KillEnemy(m_enemys[i]);
            }
        }
        for(int i = 0;i < bulletMgr.GetSearchTop();++i){
            if(bulletMgr[i].live) effMgr.Install(0,bulletMgr[i].x,bulletMgr[i].y);
        }
    }else{
        e -> live = Enemy::STOPLIVE;
        FOR_EACH(p,e->shots.begin(),e->shots.end())
            (*p) -> live = Shot::STOPSHOOT;
        collWorld.SetEnemy(e->num,false,0,0,0);
    }
}
