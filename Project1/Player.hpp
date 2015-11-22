#pragma once
#include "Spirit.hpp"
class Player :public Spirit
{
public:
	Player() :Spirit(
		BMP_ID::PLAYER,//Í¼Æ¬ÎÄ¼þID
		vector2D(0,0),  //speed
		400, //maxspeed
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