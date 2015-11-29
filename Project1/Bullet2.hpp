#pragma once
#pragma once
#include"Spirit.hpp"
#include<vector>
using namespace std;
class Bullet2 :public Spirit
{
public:
	
	Bullet2(float ax, float ay, vector2D direction = vector2D(0, -1), bool aisPlayers = true) :Spirit()
	{
		
		x = ax;
		y = ay;
		id = BMP_ID::BULLET_2;
		size = Size2D(20, 20);
		MaxSpeed = 100;
		acceleration = 100;
		attack = 200;
		Spirit::direction = direction;
		isPlayers = aisPlayers;
	
	}
	float getAngle()
	{
		return 0.f;
	}
	void action(vector<Spirit*>& Spirit_List,float DeltaTime)
	{
		Spirit::action(Spirit_List,DeltaTime);
		//ÒÆ¶¯·¶Î§ÏÞÖÆ
		float left = 0.f + size.width / 2;
		float right = 600.f - size.width / 2;
		float top = 0.f + size.height / 2;
		float bottom = 600.f - size.height / 2;
		if (x < left)hp = 0;
		if (x > right)hp = 0;
		if (y < top) hp = 0;
		if (y > bottom)hp = 0;
	}
};