#pragma once
#include"Spirit.hpp"
#include <vector>
using namespace std;
class Bullet1:public Spirit
{
public:
	
	Bullet1(float ax,float ay,vector2D direction=vector2D(0,-1),bool aisPlayers=true) :Spirit()
	{
		x = ax;
		y = ay;
		id = BMP_ID::BULLET_1;
		size = Size2D(29/2, 62/2);
		MaxSpeed = 500;
		acceleration = 10000;
		attack = 100;
		Spirit::direction=direction;
		isPlayers = aisPlayers;
	}
	
	void action(vector<Spirit*>& Spirit_List,float DeltaTime)
	{
		Spirit::action(Spirit_List,DeltaTime);
		//ÒÆ¶¯·¶Î§ÏÞÖÆ
		float left = 0.f - size.width / 2;
		float right = 600.f + size.width / 2;
		float top = 0.f - size.height / 2;
		float bottom = 600.f + size.height / 2;
		if (x < left)hp=0;
		if (x > right)hp=0;
		if (y < top) hp=0;
		if (y > bottom)hp=0;
		
	}
};