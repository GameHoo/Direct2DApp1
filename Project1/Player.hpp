#pragma once
#include "Spirit.hpp"
#include "Bullet1.hpp"
#include <vector>
using namespace std;
class Player :public Spirit
{
public:

	//飞机发射子弹频率 单位：   个/秒 
	float ShootRate=10;
	//飞机一次发射子弹数量 1-5个
	int ShootNumber = 1;
	vector<Spirit*>& Spirit_List;
	bool isTimeToShoot(float deltatime)
	{
		//累计时间
		static float time = 1 / ShootRate;
		if (time >= 1 / ShootRate)
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
   Player(vector<Spirit*>& List):Spirit_List(List)
	{
		id = BMP_ID::PLAYER;
		MaxSpeed = 300;
		acceleration = 180000;
		x = 300, y = 500;
		size = Size2D(50, 50);
		attack = 1000;        //飞机撞另一个物体伤害
	}
   float getAngle()
	{
		return 0.f;
	}
   vector<Spirit*>::iterator  action(float DeltaTime,int keydown /*
										   1 上 2下 4左 8右
										   */
	)
   {
		direction = vector2D(0, 0);
		if(keydown&1)
		{
			direction.y += -1;
		}
		if(keydown&2)
		{
			direction.y += 1;
		}
		if (keydown &4)
		{
			direction.x += -1;
		}
		if (keydown &8)
		{
			direction.x += 1;
		}
		move(DeltaTime);
		//移动范围限制
		float left = 0.f + size.width / 2;
		float right = 600.f - size.width / 2;
		float top = 0.f + size.height / 2;
		float bottom = 600.f - size.height / 2;
		if (x < left)x = left;
		if (x > right)x = right;
		if (y < top) y= top;
		if (y > bottom)y = bottom;
		//发射子弹
		if(isTimeToShoot(DeltaTime))
		{
			for (vector<Spirit*>::iterator i = Spirit_List.begin(); i != Spirit_List.end();i++)
			{
				if(*i==static_cast<Spirit*>(this))
				{
					Spirit* temp = new Bullet1(x,y-30);
					i= Spirit_List.insert(i,temp);
					return i+1;
					
				}
			}
		}
		else
		{
			for (vector<Spirit*>::iterator i = Spirit_List.begin(); i != Spirit_List.end(); i++)
			{
				if (*i == static_cast<Spirit*>(this))
				{
					return i;
				}
			}
		}
	}

};