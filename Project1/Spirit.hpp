#pragma once
#include<vector>
#include<cmath>
using namespace std;

enum BMP_ID
{
	PLAYER= 0,
	BULLET_1=1,
	ENEMY_1=2,
	BULLET_2=3
};
//表示2D矢量
class vector2D
{
	
public:
	vector2D(float ax=0,float ay=0)
	{
		x = ax;
		y = ay;
	}
	float x;
	float y;
	bool isZero()
	{
		if (fabsf(x) < 0.000001 && fabsf(y) < 0.000001)return true;
		return false;
	}
	//向量求模
	float GetModel()
	{
		return sqrt(x*x + y*y);
	}
	//向量归一化 转换成单位向量
	void VectorToOne()
	{
		float Model = GetModel();
		
		x /= Model;
		y /= Model;
	}
	//向量数乘
	vector2D operator*(float s)
	{
		vector2D temp;
		temp.x = x*s;
		temp.y = y*s;
		return temp;
	}

};
struct Size2D
{
	Size2D(int i=0,int j=0):width(i),height(j)
	{
		
	}
	int width;
	int height;
};
/*
游戏中所有物体的基类
*/
class Spirit
{
public:
	Spirit()
	{
		speed = 0; MaxSpeed = 0; acceleration = 0;
		x = 0; y = 0; direction = vector2D(0, 0); size = Size2D(0, 0);
		id = BMP_ID::PLAYER; hp = 1000; attack = 100;
	}
	//区分敌我
	bool isPlayers;
	//当前速度 单位（像素数/s）
	vector2D speed ;
	//最大速度
	float MaxSpeed;
	//加速度
	float acceleration;
	float x;
	float y;
	//方向
	vector2D direction;
	//物体大小
	Size2D size;
	//图片资源ID
	BMP_ID id;
	//生命值
	int hp ;
	//攻击力
	int attack ;
	bool isdead()
	{
		if(hp<=0)
		{
			return true;
		}
		return false;
	}
	//得到运动方向和Y轴正方向向量的夹角
	virtual	float getAngle()
	{
		vector2D temp = speed;
		float angle = 0;
		if(temp.isZero())
		{
			
		}
		else
		{
			temp.VectorToOne();
			angle = acos(-1*temp.y)*(180 / 3.1415926);
			if(speed.x<0)
			{
				angle = 360.f - angle;
			}
		}
		return angle;
	}
	//进行运动
	virtual void action(vector<Spirit*>&Spirit_List,float DeltaTime)
	{
		move(DeltaTime);
		//本精灵攻击其他精灵
		for (vector<Spirit*>::iterator i=Spirit_List.begin(); i != Spirit_List.end(); i++)
		{
			Spirit* temp = *i;
			if ((int)temp != (int)this)
			{
				if (temp->isPlayers!=isPlayers)
				{
					//碰撞检测
					if (attackTest(temp))
					{
						temp->hp -= attack;
					}
				}
			}
		}
		
	}

	void move(float DeltaTime)
	{
		if(direction.isZero())//没有方向
		{
			return;
		}
		else
		{
			//根据力的方向 计算出x y 加速度分量
			direction.VectorToOne();
			direction = direction*(acceleration*DeltaTime);

			//加速度分量加到速度中
			speed.x += direction.x;
			speed.y += direction.y;
			//最大速度限制
			if(speed.GetModel()>MaxSpeed)
			{
				//速度限制为最大速度
				speed.VectorToOne();
				speed=speed*MaxSpeed;
			}
			

			if(speed.isZero())//如果加了加速度 速度还是0
			{
				return;
			}
			else
			{
				x += speed.x*DeltaTime;
				y += speed.y*DeltaTime;
				
			}
		}
	}
	//碰撞检测
	bool attackTest(Spirit* target)
	{
		if((target->size.width/2+size.width/2)>fabs(target->x-x)&&( target->size.height / 2 + size.height / 2)>fabs(target->y - y))
		{
			return true;
		}
		return false;
	}
};