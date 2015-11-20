#pragma once
#include<math.h>
//表示2D矢量
class vector2D
{
public:
	float x;
	float y;
	//向量求模
	float GetModel()
	{
		return sqrt(x*x + y*y);
	}
	//向量归一化 转换成单位向量
	void VectorToOne()
	{
		float Model = GetModel();
		if (Model < 0.0001)return;//超级小向量 无视
		x /= Model;
		y /= Model;
	}
	//向量数乘
	vector2D operator+(float s)
	{
		vector2D temp;
		temp.x = x*s;
		temp.y = y*s;
		return temp;
	}
};
/*
游戏中所有物体的基类
*/
class Spirit
{

protected:
	//当前速度 单位（像素数/s）
	float speed=0;
	//最大速度
	float MaxSpeed = 0;
	//加速度
	float acceleration = 0;
public:
	float x=0;
	float y=0;
	//进行运动
	void move()
	{
		
	}
};