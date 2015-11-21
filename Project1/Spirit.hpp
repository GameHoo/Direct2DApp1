#pragma once
#include<math.h>
enum BMP_ID
{
	PLAYER= 0,
	BULLET_1=1

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
	Spirit(BMP_ID aid=BMP_ID::PLAYER, vector2D aspeed=vector2D(0,0),float aMaxSpeed=0,float aacceleration=0,float ax=0,float ay=0,vector2D aDirectionVector =0,Size2D asize=0)
		:
		id(aid),
		speed(aspeed),
		MaxSpeed(aMaxSpeed),
		acceleration(aacceleration),
		x(ax),
		y(ay),
		directionvector(aDirectionVector),
		size(asize)
	{
		
	}
	//当前速度 单位（像素数/s）
	vector2D speed ;
	//最大速度
	float MaxSpeed;
	//加速度
	float acceleration;
	float x;
	float y;
	//方向
	vector2D directionvector;
	//物体大小
	Size2D size;
	//图片资源ID
	BMP_ID id;
	//得到运动方向和Y轴正方向向量的夹角
	float getAngle()
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
	void move(float DeltaTime)
	{
		if(directionvector.isZero())//没有方向
		{
			return;
		}
		else
		{
			//根据力的方向 计算出x y 加速度分量
			directionvector.VectorToOne();
			directionvector = directionvector*(acceleration*DeltaTime);

			//加速度分量加到速度中
			speed.x += directionvector.x;
			speed.y += directionvector.y;
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
};