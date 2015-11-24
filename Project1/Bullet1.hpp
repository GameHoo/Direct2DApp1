#pragma once
#include"Spirit.hpp"
class Bullet1:public Spirit
{
public:
	Bullet1() :Spirit(
		BMP_ID::BULLET_1,//Í¼Æ¬ÎÄ¼þID
		vector2D(0, 0),  //speed
		500, //maxspeed
		10000, //acceleration
		400,//x
		550,//y
		vector2D(0, -1),//direction
		Size2D(3, 15),//size
		1000,//hp
		200//attack
		)
	{}
	float getAngle()
	{
		return 0.f;
	}

};