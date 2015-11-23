#pragma once
#include "Snow.h"

class PlayerBullet{
	public:
	void Add(double x,double y,double power,int style);
	void OnNext();
	void OnDraw();
	void Clear();
	void Kill(int num);
	private:
	inline void setBullet(int num,double x,double y,double power,int style);

	int m_searchTop = 0;
	struct PlayerBulletObj{
		int style=-1;    //-1 == DEATH
		double x,y,pow;
	}m_bullets[512];

	struct PlayerBulletStyle{
		SDL_Texture* tex;
		int w,h;
	}m_bulletStyles[2];

};
