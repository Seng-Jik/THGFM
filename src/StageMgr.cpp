#include "StageMgr.h"
#include "Snow.h"
#include "Tools.h"
#include <cmath>
#include "Snow/Debug.h"
#include "EnePartten.h"
#include "ShotParttenAPI.h"
#include "CollWorld.h"
using namespace Snow;

StageMgr stage;

StageMgr::EnemyStyle StageMgr::m_eStyles [1];

void StageMgr::Init()
{
    m_eStyles[0].tex = LoadPic("Enemy/e0.png");
}


StageMgr::StageMgr()
{
    //ctor
}

StageMgr::~StageMgr()
{
    //dtor
}

void StageMgr::LoadCSV(const std::string& stage)
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

}

void StageMgr::OnNext()
{
    ++m_cnt;
    for(int i = m_enemySearchTop;i < (int)m_enemys.size();++i){
        if(m_cnt == m_enemys[i]->birth){
            m_enemySearchTop=i+1;
            collWorld.SetEnemySearchTop(i+1);
            m_enemys[i]->cnt = 0;
            m_enemys[i]->live = Enemy::LIVE;
        }else break;
    }

    for(int i = m_enemySearchBottom;i < m_enemySearchTop;++i){
        bool allShotDied = true;
        FOR_EACH(pShot,m_enemys[i]->shots.begin(),m_enemys[i]->shots.end()){
            Shot& shot = **pShot;

            if(shot.live == Shot::STOPSHOOT){
                bool noBullet = true;
                for(int i = 0;i < (int)shot.bullets.size();++i)
                    if(shot.bullets[i] != -1) {noBullet = false;break;}
               if(noBullet) shot.live = Shot::DEATH;
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
            if((shot.live == Shot::LIVE || shot.live == Shot::STOPSHOOT) && shot.partten != -1)
                (*shotPartten[shot.partten])(&shot,i);

            ++shot.cnt;
        }

        if(m_enemys[i]->live == Enemy::LIVE){
            //TODO:敌人模式
            if(m_enemys[i] -> partten != -1)
                enemyPartten[m_enemys[i] -> partten](m_enemys[i],i);

            ++m_enemys[i] -> cnt;
            m_enemys[i] -> y -= m_enemys[i] ->spd * sin(m_enemys[i] -> angle);
            m_enemys[i] -> x -= m_enemys[i] ->spd * cos(m_enemys[i] -> angle);

            collWorld.SetEnemy(i,true,m_enemys[i]->x,m_enemys[i] ->y,m_eStyles[m_enemys[i]->style].r);

            if((m_enemys[i] -> x < -2*m_eStyles[m_enemys[i] -> style].r ||
                m_enemys[i] -> x > WIDTH + m_eStyles[m_enemys[i] -> style].r||
                m_enemys[i] -> y > HEIGHT + m_eStyles[m_enemys[i] -> style].r||
                m_enemys[i] -> y < -2*m_eStyles[m_enemys[i] -> style].r)&&
               m_enemys[i] ->cnt >= 180
                )
                    KillEnemy(m_enemys[i]);

            if(allShotDied && m_enemys[i]->live == Enemy::STOPLIVE)
                m_enemys[i] -> live = Enemy::DEATH;
        }

        //SDL_assert(allShotDied);
        //PNT("ESBC:"<<allShotDied<<(m_enemys[i]->live == Enemy::DEATH)<<(i == m_enemySearchBottom)<<m_enemySearchBottom);
        if(allShotDied && m_enemys[i]->live == Enemy::STOPLIVE && i >= m_enemySearchBottom){
            FOR_EACH(p,m_enemys[i]->shots.begin(),m_enemys[i]->shots.end())
                delete *p;
            m_enemys[i]->shots.clear();
            m_enemySearchBottom = i+1;
            collWorld.SetEnemySearchBottom(i+1);
        }


    }
    collWorld.Update_Player_Enemy();
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
}

void StageMgr::KillEnemy(Enemy* e)
{
    e -> live = Enemy::STOPLIVE;
    FOR_EACH(p,e->shots.begin(),e->shots.end())
        (*p) -> live = Shot::STOPSHOOT;

    collWorld.SetEnemy(e -> num,false);
}
