#include "BulletMgr.h"
#include "Tools.h"
#include <cmath>
#include "CollWorld.h"
#include <cmath>
#include "Snow/Debug.h"
using namespace Snow;

BulletMgr bulletMgr;

void BulletMgr::enableXRectBullet(int bltNum)
{
    //TODO：Debug项目，四边形子弹的旋转角对四个点的转换并设置到碰撞盒
    int x[4],y[4],x2[4];
    double theta = m_blts[bltNum].self_angle;
    //theta = 2*M_PI - theta;
    x[0] = -m_blts[bltNum].self_roll_center_x;
    y[0] = -m_blts[bltNum].self_roll_center_y;
    x[1] = -m_blts[bltNum].self_roll_center_x;
    y[1] = m_blts[bltNum].self_roll_center_y;
    x[2] = m_blts[bltNum].self_roll_center_x;
    y[2] = m_blts[bltNum].self_roll_center_y;
    x[3] = m_blts[bltNum].self_roll_center_x;
    y[3] = -m_blts[bltNum].self_roll_center_y;
    /*x[0] = 0;
    y[0] = 0;
    x[1] = 0;
    y[1] = m_bulletStyle[m_blts[bltNum].style].xrect.h;
    x[2] = m_bulletStyle[m_blts[bltNum].style].xrect.w;
    y[2] = m_bulletStyle[m_blts[bltNum].style].xrect.h;
    x[3] = m_bulletStyle[m_blts[bltNum].style].xrect.w;
    y[3] = 0;*/

    for(int i = 0;i < 4;++i){
        x2[i] =x[i]*cos(theta) - y[i]*sin(theta);
        y[i] = x[i]*sin(theta) + y[i]*cos(theta);
        x2[i] += m_blts[bltNum].x;
        y[i] += m_blts[bltNum].y;//-m_blts[bltNum].self_h;
    }
    collWorld.SetEnemyBulletXRect(bltNum,true,x2,y);
}

void BulletMgr::loadCircleBulletStyle(int i, const std::string& s, double r, double coll_r)
{
    m_bulletStyle[i].tex = LoadPic(s);
    SDL_SetTextureBlendMode(m_bulletStyle[i].tex,SDL_BLENDMODE_BLEND);
    m_bulletStyle[i].shape = BulletStyle::CIRCLE;
    m_bulletStyle[i].circle.r = r;
    m_bulletStyle[i].circle.coll_r = coll_r;
}

void BulletMgr::loadXRectBulletStyle(int i, const std::string& s, int w, int h)
{
    m_bulletStyle[i].tex = LoadPic(s);
    SDL_SetTextureBlendMode(m_bulletStyle[i].tex,SDL_BLENDMODE_BLEND);
    m_bulletStyle[i].shape = BulletStyle::XRECT;
    m_bulletStyle[i].xrect.w = w;
    m_bulletStyle[i].xrect.h = h;
}


BulletMgr::BulletStyle BulletMgr::m_bulletStyle[64];
void BulletMgr::Init()
{
    Snow::CSVReader csv;
    csv.LoadCSV("Bullet/styles.csv");
    std::string shape,file;
    int num = 0;
    csv.NextLine();
    do{
        double r1,r2;
        csv.PopStr(shape);
        csv.PopStr(file);
        file = "Bullet/" + file;
        csv.PopFloat(r1);
        csv.PopFloat(r2);
        if(shape[0] == 'C'){
            loadCircleBulletStyle(num,file.c_str(),r1,r2);
        }else if(shape[0] == 'R'){
            loadXRectBulletStyle(num,file.c_str(),r1,r2);
        }else if(shape[0] == '#');
        ++num;
    }while(csv.NextLine());
}


BulletMgr::BulletMgr()
{
    Clear();
}

BulletMgr::~BulletMgr()
{
    //dtor
}

void BulletMgr::OnNext()
{
    for(int i = 0;i < m_searchTop;++i){
        if(m_blts[i].live){
            if(m_blts[i].alpha < 250 && m_blts[i].alpha_living){
                m_blts[i].alpha+=25;
                if(m_blts[i].alpha == 255) m_blts[i].alpha_living = false;
            }
            m_blts[i].x -= m_blts[i].spd * cos(m_blts[i].angle);
            m_blts[i].y -= m_blts[i].spd * sin(m_blts[i].angle);
            ++m_blts[i].cnt;
            switch(m_bulletStyle[m_blts[i].style].shape){
            case BulletStyle::CIRCLE:
                collWorld.SetEnemyBulletCircle(i,true,m_blts[i].x,m_blts[i].y,m_bulletStyle[m_blts[i].style].circle.coll_r);
                break;
            case BulletStyle::XRECT:
                //TODO:SetRectColler
                enableXRectBullet(i);
                break;
            }

            bool needKill = false;
            if(m_bulletStyle[m_blts[i].style].shape == BulletStyle::CIRCLE)
                needKill = m_blts[i].minLiveTime <= m_blts[i].cnt && (
                    m_blts[i].x > WIDTH+ m_bulletStyle[m_blts[i].style].circle.r||
                    m_blts[i].x < -m_bulletStyle[m_blts[i].style].circle.r ||
                    m_blts[i].y > HEIGHT+ m_bulletStyle[m_blts[i].style].circle.r||
                    m_blts[i].y < -m_bulletStyle[m_blts[i].style].circle.r);

            else if(m_bulletStyle[m_blts[i].style].shape == BulletStyle::XRECT){
                int bigEdge = m_bulletStyle[m_blts[i].style].xrect.w > m_bulletStyle[m_blts[i].style].xrect.h ? m_bulletStyle[m_blts[i].style].xrect.w : m_bulletStyle[m_blts[i].style].xrect.h;
                ++bigEdge;
                needKill = m_blts[i].minLiveTime <= m_blts[i].cnt && (
                    m_blts[i].x > WIDTH+ bigEdge||
                    m_blts[i].x < -bigEdge ||
                    m_blts[i].y > HEIGHT+ bigEdge||
                    m_blts[i].y < -bigEdge);
            }

            if(needKill) Kill(i);
        }
    }
}
#include "Snow/Debug.h"
void BulletMgr::OnDraw()
{
    for(int i = 0;i < m_searchTop;++i){
        if(m_blts[i].live && m_bulletStyle[m_blts[i].style].shape == BulletStyle::CIRCLE){
            SDL_Rect r = {int(m_blts[i].x - m_bulletStyle[m_blts[i].style].circle.r),
                                int(m_blts[i].y - m_bulletStyle[m_blts[i].style].circle.r),
                                int(2*m_bulletStyle[m_blts[i].style].circle.r),
                                int(2*m_bulletStyle[m_blts[i].style].circle.r)
            };
            SDL_SetTextureAlphaMod(m_bulletStyle[m_blts[i].style].tex,m_blts[i].alpha);
            SDL_Point poi = {int(m_bulletStyle[m_blts[i].style].circle.r),int(m_bulletStyle[m_blts[i].style].circle.r)};
            //PNT("DRAW BULLET:"<<m_blts[i].roll_x<<" "<<m_blts[i].roll_y);
            //SDL_Point poi = {8,8};
            SDL_RenderCopyEx(pRnd,m_bulletStyle[m_blts[i].style].tex,nullptr,&r,m_blts[i].self_angle * 180/M_PI,&poi,SDL_FLIP_NONE);
            //SDL_RenderCopy(pRnd,m_bulletStyle[m_blts[i].style].tex,nullptr,&r);
        }
        else if(m_blts[i].live && m_bulletStyle[m_blts[i].style].shape == BulletStyle::XRECT){
            SDL_Rect r = {int(m_blts[i].x-m_blts[i].self_w/2),
                                int(m_blts[i].y-m_blts[i].self_h/2),
                                int(m_blts[i].self_w),
                                int(m_blts[i].self_h)
            };
            SDL_Point roll_poi = {m_blts[i].self_roll_center_x,m_blts[i].self_roll_center_y};
            //SDL_SetTextureAlphaMod(m_bulletStyle[m_blts[i].style].tex,128);
            SDL_SetTextureAlphaMod(m_bulletStyle[m_blts[i].style].tex,m_blts[i].alpha);
            //SDL_Point poi = {int(m_bulletStyle[m_blts[i].style].xrect.w/2),int(m_bulletStyle[m_blts[i].style].xrect.h/2)};
            SDL_RenderCopyEx(pRnd,m_bulletStyle[m_blts[i].style].tex,nullptr,&r,m_blts[i].self_angle * 180/M_PI,&roll_poi,SDL_FLIP_NONE);
        }
    }
}

void BulletMgr::Clear()
{
    while(!m_freeList.empty()) m_freeList.pop();
    for(int i = 0;i < 1500;++i){
        m_freeList.push(i);
        m_blts[i].live = false;
    }
    m_searchTop = 0;
    collWorld.ClearEnemyBullet();
}

int BulletMgr::Alloc(double x,double y,int style)
{
    if(m_freeList.empty()) return -1;
    int n = 1499 - m_freeList.top();
    m_freeList.pop();
    if(n >= m_searchTop) m_searchTop = n+1;
    collWorld.SetEnemyBulletSearchTop(m_searchTop);

    m_blts[n].style = style;
    m_blts[n].live = true;
    m_blts[n].cnt = 0;
    m_blts[n].alpha = 0;
    m_blts[n].alpha_living = true;
    m_blts[n].self_w = m_bulletStyle[style].xrect.w;
    m_blts[n].self_h = m_bulletStyle[style].xrect.h;
    switch(m_bulletStyle[style].shape){
    case BulletStyle::XRECT:
        m_blts[n].x = x;
        m_blts[n].y = y;
        m_blts[n].self_roll_center_x = m_blts[n].self_w/2;
        m_blts[n].self_roll_center_y = m_blts[n].self_h/2;
        break;
    case BulletStyle::CIRCLE:
        m_blts[n].x = m_bulletStyle[style].circle.r/2+x;
        m_blts[n].y = m_bulletStyle[style].circle.r/2+y;
        break;
    }

    /*if(m_bulletStyle[n].shape == BulletStyle::CIRCLE){
        m_blts[n].r_x = m_blts[n].r_y = int(m_bulletStyle[style].circle.r);
        PNT("DRAW BULLET:"<<m_blts[n].r_x<<" "<<m_blts[n].r_y);
    }
    else if(m_bulletStyle[n].shape == BulletStyle::XRECT){
        m_blts[n].r_x = m_bulletStyle[style].xrect.w/2;
        m_blts[n].r_y = m_bulletStyle[style].xrect.h/2;
    }*/

    return n;
}

void BulletMgr::Kill(int n)
{
    if(m_blts[n].link){ //如果由普通射击管理
        if(m_blts[n].link -> live == Shot::LIVE) m_blts[n].link -> bullets[m_blts[n].linkNum] = -1;
    }else{  //如果由Boss模式管理
        stage.GetBoss() ->m_bullets[m_blts[n].linkNum] = -1;
    }
    m_blts[n].live = false;
    m_freeList.push(1499-n);
    while(!m_blts[m_searchTop - 1].live)
        --m_searchTop;

    switch(m_bulletStyle[m_blts[n].style].shape){
    case BulletStyle::CIRCLE:
        collWorld.SetEnemyBulletCircle(n,false);
        break;
    case BulletStyle::XRECT:
        collWorld.SetEnemyBulletXRect(n,false);
        break;
    }

    collWorld.SetEnemyBulletSearchTop(m_searchTop);
}

#include "EffectMgr.h"
void BulletMgr::KillBulletAndInstallEffect(int n)
{
    int i = n;
    if(m_bulletStyle[m_blts[i].style].shape == BulletStyle::XRECT){
            SDL_Rect r = {int(m_blts[i].x-m_blts[i].self_w/2),
                                int(m_blts[i].y-m_blts[i].self_h/2),
                                int(m_blts[i].self_w),
                                int(m_blts[i].self_h)
            };
            SDL_Point roll_poi = {m_blts[i].self_roll_center_x,m_blts[i].self_roll_center_y};
            SDL_SetTextureAlphaMod(m_bulletStyle[m_blts[i].style].tex,m_blts[i].alpha);
            effMgr.InstallZoomOutAnimation(m_bulletStyle[m_blts[n].style].tex,r,true,roll_poi,m_blts[i].self_angle* 180/M_PI);
    }
    else if(m_blts[i].live && m_bulletStyle[m_blts[i].style].shape == BulletStyle::CIRCLE){
        SDL_Rect r = {int(m_blts[i].x - m_bulletStyle[m_blts[i].style].circle.r),
                            int(m_blts[i].y - m_bulletStyle[m_blts[i].style].circle.r),
                            int(2*m_bulletStyle[m_blts[i].style].circle.r),
                            int(2*m_bulletStyle[m_blts[i].style].circle.r)
        };
        SDL_SetTextureAlphaMod(m_bulletStyle[m_blts[i].style].tex,m_blts[i].alpha);
        SDL_Point poi = {int(m_bulletStyle[m_blts[i].style].circle.r),int(m_bulletStyle[m_blts[i].style].circle.r)};
        effMgr.InstallZoomOutAnimation(m_bulletStyle[m_blts[n].style].tex,r,true,poi,m_blts[i].self_angle* 180/M_PI);
    }
    Kill(n);
}
