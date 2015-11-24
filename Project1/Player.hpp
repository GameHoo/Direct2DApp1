#pragma once
#include "Spirit.hpp"
class Player :public Spirit
{
public:
	//飞机发射子弹频率 单位：   个/秒 
	float ShootRate=10;
	//飞机一次发射子弹数量 1-5个
	int ShootNumber = 1;
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
	Player() :Spirit(
		BMP_ID::PLAYER,//图片文件ID
		vector2D(0,0),  //speed
		300, //maxspeed
		180000, //acceleration
		300,//x
		550,//y
		vector2D(0,0),//direction
		Size2D(50,50),//size
		1000,//hp
		1000//attack
		)
	{}
   float getAngle()
	{
		return 0.f;
	}

};