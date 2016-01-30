#include "ItemMgr.h"
#include "Tools.h"
#define IMG_WIDTH 16
#include "Snow.h"
#define PNT(x)
using namespace Snow;

ItemMgr itemMgr;
SDL_Texture* ItemMgr::m_imgs[5];
void ItemMgr::Init()
{
    m_imgs[POWER] = LoadPic("Item/Power.png");
    m_imgs[FULLPOWER] = LoadPic("Item/FullPower.png");
    m_imgs[LIFE1UP] = LoadPic("Item/Life.png");
    m_imgs[SCORE] = LoadPic("Item/Score.png");
    m_imgs[BOMB] = LoadPic("Item/Bomb.png");
}

void ItemMgr::OnDraw()
{
    for(int i = 0;i < m_searchTop;++i){
        if(m_items[i].live){
            SDL_Rect r = {(int)m_items[i].x,(int)m_items[i].y,IMG_WIDTH,IMG_WIDTH};
            SDL_RenderCopy(pRnd,m_imgs[m_items[i].type],nullptr,&r);
        }
    }
}
#include "Player.h"
#include <cmath>
#include "GameUI.h"
void ItemMgr::OnNext()
{
    for(int i = 0;i < m_searchTop;++i){
        if(m_items[i].live){
            if(m_items[i].state == Item::DOWNING){
                m_items[i].y += m_items[i].ySpd;
                m_items[i].ySpd += 0.05;

                m_items[i].y -= 2;
                m_items[i].x -= m_items[i].boomSpdX;
                if(m_items[i].boomSpdX-0.1>0.9) m_items[i].boomSpdX -= 0.1;
                if(m_items[i].boom <= 0) m_items[i].state = Item::DOWNING;

                if(m_items[i].y >= HEIGHT) Kill(i);

                /* 收取开始 */
                for(int p = 0;p < 2;++p){
                    double x,y;
                    player[m_items[i].gettingToPlayer].GetPos(x,y);
                    if(!player[m_items[i].gettingToPlayer].Living()) continue;
                    if(x+60 > m_items[i].x && x - 60 < m_items[i].x &&
                        y+60 > m_items[i].y && y - 60 < m_items[i].y) GetItem(i,p);
                }

            }else if(m_items[i].state == Item::GETTING){
                /* 收取模式 */
                if(!player[m_items[i].gettingToPlayer].Living()) m_items[i].state = Item::DOWNING;
                double x,y;
                player[m_items[i].gettingToPlayer].GetPos(x,y);
                if(m_items[i].gettingSpd < 40) m_items[i].gettingSpd += 2;
                double angle = atan2(m_items[i].y - y,m_items[i].x - x);
                m_items[i].y -= m_items[i].gettingSpd * sin(angle);
                m_items[i].x -= m_items[i].gettingSpd * cos(angle);

                /* 如果与玩家重合 */
                //TODO:1 Player Mode
                if(!player[m_items[i].gettingToPlayer].Living()) continue;
                if(x+20 > m_items[i].x && x - 20 < m_items[i].x &&
                    y+20 > m_items[i].y && y - 20 < m_items[i].y){
                        //TODO:Handle item event.
                        switch(m_items[i].type){
                        case POWER:
                            player[m_items[i].gettingToPlayer].AddPower(0.0005);break;
                        case FULLPOWER:
                            player[m_items[i].gettingToPlayer].AddPower(4);break;
                        case BOMB:
                            gameUI.OnGetBomb();break;
                        case LIFE1UP:
                            gameUI.OnGetLife();break;
                        };
                        Kill(i);
                }

            }
        }
    }
}

void ItemMgr::AddItem(ItemType i, double boom, int x, int y, int num)
{
    for(int cnt = 0;cnt < num;++cnt){
        int n = m_freeList.top();
        m_freeList.pop();
        if(n >= m_searchTop)
            m_searchTop = n+1;

        m_items[n].live = true;
        m_items[n].x = x;
        m_items[n].y = y;
        m_items[n].ySpd = -boom/4+boom/8*(float)(rand()%num)/num;
        m_items[n].type = i;
        m_items[n].state = Item::DOWNING;
        m_items[n].boomSpdX = boom/4+boom/4*(float)(rand()%num)/num;
        PNT("ADD"<<m_searchTop);
    }
}

void ItemMgr::Clear()
{
    while(!m_freeList.empty()) m_freeList.pop();
    for(int i = 0;i < 750;++i){
        m_freeList.push(i);
        m_items[i].live = false;
    }
    m_searchTop = 0;
}

void ItemMgr::GetAll(int playerNum)
{
    for(int i = 0;i < m_searchTop;++i)
        if(m_items[i].live) GetItem(i,playerNum);
}

void ItemMgr::GetItem(int num,int playerNum)
{
    if(m_items[num].state == Item::GETTING) return;
    m_items[num].state = Item::GETTING;
    m_items[num].gettingSpd = 0;
    m_items[num].gettingToPlayer = playerNum;
}

void ItemMgr::Kill(int num){
    m_items[num].live = false;
    m_freeList.push(num);
    while(!m_items[m_searchTop-1].live && m_searchTop > 0)
        --m_searchTop;
    PNT("KILL"<<m_searchTop);
}
