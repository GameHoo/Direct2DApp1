#pragma once
#include<math.h>
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
		if (abs(x) < 0.000001 && abs(y) < 0.000001)return true;
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
	Size2D():width(0),height(0)
	{
		
	}
	int width;
	int height;
};
/*
游戏中所有物体的基类
*/
class spirit
{
public:
	spirit()
	{
		
	}
	//当前速度 单位（像素数/s）
	float speed = 0;
	//最大速度
	float MaxSpeed = 0;
	//加速度
	float acceleration = 0;
	float x=0;
	float y=0;
	//方向
	vector2D directionvector;
	//物体大小
	Size2D size;
	//进行运动
	void move(float DeltaTime)
	{
		if(directionvector.isZero())
		{
			return;
		}
		else
		{
			directionvector.VectorToOne();
			directionvector = directionvector*(speed*DeltaTime);

			if(directionvector.isZero())
			{
				return;
			}
			else
			{
				x += directionvector.x;
				y += directionvector.y;
			}
		}
	}
};