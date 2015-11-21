#pragma once
#include "Spirit.hpp"
class Player :public Spirit
{
public:
	Player() :Spirit(
		BMP_ID::PLAYER,//Í¼Æ¬ÎÄ¼þID
		vector2D(0,0),  //speed
		300, //maxspeed
		800, //acceleration
		400,//x
		550,//y
		vector2D(0,0),//direction
		Size2D(50,50)//size
		)
	{

	}
	

};