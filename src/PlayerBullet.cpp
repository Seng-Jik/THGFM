#include "PlayerBullet.h"
#include "Tools.h"
#include "CollWorld.h"

using namespace Snow;

PlayerBullet playerBulletMgr;
PlayerBullet::PlayerBulletStyle PlayerBullet::m_bulletStyles[2];

void PlayerBullet::Init()
{
    m_bulletStyles[1].tex = LoadPic("Player/Marisa/bullet.png");
    m_bulletStyles[0].tex = LoadPic("Player/Reimu/bullet.png");
    m_bulletStyles[0].w = 27;
    m_bulletStyles[0].h = 15;
    m_bulletStyles[1].w = 46;
    m_bulletStyles[1].h = 15;
}


inline void PlayerBullet::setBullet(int num,double x,double y,double power,int style){
	//Sync CollWorld
    if(m_bullets[num].style != -1)
        collWorld.SetPlayerBullet(num,true,x,x,y,m_bulletStyles[m_bullets[num].style].w,m_bulletStyles[m_bullets[num].style].h,m_searchTop);

    //Set Bullet
    m_bullets[num].x=x;
	m_bullets[num].y=y;
	m_bullets[num].pow=power;
	m_bullets[num].style=style;
}

void PlayerBullet::Add(double x,double y,double power,int style){
	for(int i=0;i<m_searchTop;++i)
		if(m_bullets[i].style == -1){
			setBullet(i,x,y,power,style);
			return;
		}
	setBullet(m_searchTop,x,y,power,style);
	++m_searchTop;
}

void PlayerBullet::Kill(int num){
	m_bullets[num].style=-1;

	while(m_bullets[m_searchTop-1].style == -1)
	    --m_searchTop;

    //Sync CollWorld
    collWorld.SetPlayerBullet(num,false,0,0,0,0,0,m_searchTop);
}

void PlayerBullet::Clear(){
	for(int i=0;i<m_searchTop;++i){
	    m_bullets[i].style=-1;
	    collWorld.SetPlayerBullet(i,false,0,0,0,0,0,0);
	}
	m_searchTop=-1;

}

void PlayerBullet::OnDraw(){
	for(int i=0;i<m_searchTop;++i)
		if(m_bullets[i].style != -1){
		    const PlayerBulletStyle& style = m_bulletStyles[m_bullets[i].style];
		    SDL_Rect dst = {
		    	int(m_bullets[i].x - style.w/2),
		    	int(m_bullets[i].y - style.h/2),
		    	style.w,
		    	style.h
		    };
		    SDL_RenderCopy(pRnd,style.tex,nullptr,&dst);
		}
}

void PlayerBullet::OnNext(){
    int bestTop = HEIGHT;
    int bestBottom = 0;
	for(int i=0;i<m_searchTop;++i)
		if(m_bullets[i].style != -1){
			PlayerBulletObj& bul = m_bullets[i];
			const PlayerBulletStyle& style = m_bulletStyles[bul.style];

			//子弹运行
			bul.x += 45;

			//Sync Playet Bullet CollWorld
			collWorld.SetPlayerBullet(i,true,bul.x,bul.x-50,m_bulletStyles[bul.style].w,bul.y,m_bulletStyles[bul.style].h,m_searchTop);
			//碰撞世界的判定方法：
			/*
			每次同步时：
			刚体对象记录上次的x值和本次的x值。

			每次判定时：
			刚体被拉长，设原x位置为lx，新位置x,y，长宽w,h
			则碰撞范围为 lx+w/2,y-h/2,x-lx,h
			使用方对圆判定
			*/

			if(bul.x - style.w/2 >= WIDTH){
				//子弹超出屏幕判定
				//仅适用于向右直线运动的子弹
				Kill(i);
			}else{
                if(bul.y - m_bulletStyles[bul.style].h < bestTop) bestTop = bul.y - m_bulletStyles[bul.style].h;
                if(bul.y + 2*m_bulletStyles[bul.style].h > bestBottom) bestBottom = bul.y + 2*m_bulletStyles[bul.style].h;
			}
		}
        collWorld.SetPlayerBulletYRect(bestTop,bestBottom);

}
