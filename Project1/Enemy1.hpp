#pragma once
#include"Spirit.hpp"
#include <vector>
#include <stdlib.h>
class Enemy1 :public Spirit
{
public:
	Enemy1(float ax, float ay, vector2D direction = vector2D(0, 1)) :Spirit()
	{
		x = ax;
		y = ay;
		id = BMP_ID::ENEMY_1;
		size = Size2D(107/2, 80/2);
		MaxSpeed = 100;
		acceleration = 800;
		attack = 200;
		Spirit::direction = direction;
		isPlayers = false;
	}
	float getAngle()
	{
		return 0.f;
	}
	bool isTimeToShoot(float deltatime)
	{
		
		//累计时间
		static float time = 0;
		srand(time);
		if (time >= 5.f)
		{
			time = 0;
			return true;
		}
		else
		{
			time += deltatime;
			return false;
		}
	}
	void action(vector<Spirit*>& Spirit_List, vector<Spirit*>::iterator &i,float DeltaTime)
	{
		Spirit::action(Spirit_List,DeltaTime);
		//移动范围限制
		float left = 0.f - size.width / 2;
		float right = 600.f + size.width / 2;
		float top = 0.f - size.height / 2;
		float bottom = 600.f + size.height / 2;
		if (x < left)hp = 0;
		if (x > right)hp = 0;
		if (y < top) hp = 0;
		if (y > bottom)hp = 0;
		//发射子弹
		if (isTimeToShoot(DeltaTime))
		{
			Spirit* temp = new Bullet1(x, y,vector2D(0,1),false);
			i = Spirit_List.insert(i, temp);
			i=i + 2;
			return;
		}
		else
		{
			i++;
			return;
		}
	}
};