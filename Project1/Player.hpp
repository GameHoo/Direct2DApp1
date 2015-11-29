#pragma once
#include "Spirit.hpp"
#include "Bullet1.hpp"
#include <vector>
using namespace std;
class Player :public Spirit
{
public:

	//�ɻ������ӵ�Ƶ�� ��λ��   ��/�� 
	float ShootRate=10;
	//�ɻ�һ�η����ӵ����� 1-5��
	int ShootNumber = 1;
	bool isTimeToShoot(float deltatime)
	{
		//�ۼ�ʱ��
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
   Player()
	{
		id = BMP_ID::PLAYER;
		MaxSpeed = 300;
		acceleration = 180000;
		x = 300, y = 500;
		size = Size2D(129/2, 104/2);
		attack = 1000;        //�ɻ�ײ��һ�������˺�
		isPlayers = true;
	}
   /*float getAngle()
	{
		return 0.f;
	}*/
   bool shoot(wstring& bullet_ID, int& bullet_Number, int& bullet_Source_x, int& bullet_Source_y, bool bullet_IsEnemy,float deltatime)
   {
	   if(!isTimeToShoot(deltatime))
	   {
		   return false;
	   }
	   bullet_ID = L"bullet1";
	   bullet_Number = ShootNumber;
	   bullet_Source_x = x;
	   bullet_Source_y = y;
	   bullet_IsEnemy = false;
	   return true;
   }
  void action(vector<Spirit*>&Spirit_List, vector<Spirit*>::iterator &i,float DeltaTime,int keydown)
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
		Spirit::action(Spirit_List, DeltaTime);
		//�ƶ���Χ����
		float left = 0.f + size.width / 2;
		float right = 600.f - size.width / 2;
		float top = 0.f + size.height / 2;
		float bottom = 600.f - size.height / 2;
		if (x < left)x = left;
		if (x > right)x = right;
		if (y < top) y= top;
		if (y > bottom)y = bottom;
		//�����ӵ�
		if(isTimeToShoot(DeltaTime))
		{
			Spirit* temp = new Bullet1(x,y-45);
			i= Spirit_List.insert(i,temp);
			i=i+2;
			return;
		}
		else
		{
			i++;
			return;
		}
	}

};