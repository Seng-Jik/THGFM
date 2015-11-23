#include "PlayerBullet.h"
#include "Tools.h"

using namespace Snow;

inline void PlayerBullet::setBullet(int num,double x,double y,double power,int style){
	m_bullets[num].x=x;
	m_bullets[num].y=y;
	m_bullets[num].pow=power;
	m_bullets[num].style=style;
	//todo:Sync CollWorld
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
	//Sync CollWorld

	while(m_bullets[m_searchTop-1].style == -1)
	    --m_searchTop;
}

void PlayerBullet::Clear(){
	for(int i=0;i<m_searchTop;++i)
	    m_bullets[i].style=-1;
	m_searchTop=-1;
	//Clear Player Bullets CollWorld
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
	for(int i=0;i<m_searchTop;++i)
		if(m_bullets[i].style != -1){
			PlayerBulletObj& bul = m_bullets[i];
			const PlayerBulletStyle& style = m_bulletStyles[bul.style];

			//子弹运行
			bul.x += 5;
			//todo:Sync Playet Bullet CollWorld
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
			}
		}

}
