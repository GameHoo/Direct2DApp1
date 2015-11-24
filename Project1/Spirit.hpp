#pragma once

#include<cmath>
using namespace std;

enum BMP_ID
{
	PLAYER= 0,
	BULLET_1=1,
	ENEMY_1=2
};
//��ʾ2Dʸ��
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
	//������ģ
	float GetModel()
	{
		return sqrt(x*x + y*y);
	}
	//������һ�� ת���ɵ�λ����
	void VectorToOne()
	{
		float Model = GetModel();
		
		x /= Model;
		y /= Model;
	}
	//��������
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
��Ϸ����������Ļ���
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
	//��ǰ�ٶ� ��λ��������/s��
	vector2D speed ;
	//����ٶ�
	float MaxSpeed;
	//���ٶ�
	float acceleration;
	float x;
	float y;
	//����
	vector2D direction;
	//�����С
	Size2D size;
	//ͼƬ��ԴID
	BMP_ID id;
	//����ֵ
	int hp ;
	//������
	int attack ;
	bool isdead()
	{
		if(hp<=0)
		{
			return true;
		}
		return false;
	}
	//�õ��˶������Y�������������ļн�
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
	//�����˶�
	virtual void action(float DeltaTime)
	{
		move(DeltaTime);
	}
	void move(float DeltaTime)
	{
		if(direction.isZero())//û�з���
		{
			return;
		}
		else
		{
			//�������ķ��� �����x y ���ٶȷ���
			direction.VectorToOne();
			direction = direction*(acceleration*DeltaTime);

			//���ٶȷ����ӵ��ٶ���
			speed.x += direction.x;
			speed.y += direction.y;
			//����ٶ�����
			if(speed.GetModel()>MaxSpeed)
			{
				//�ٶ�����Ϊ����ٶ�
				speed.VectorToOne();
				speed=speed*MaxSpeed;
			}
			

			if(speed.isZero())//������˼��ٶ� �ٶȻ���0
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