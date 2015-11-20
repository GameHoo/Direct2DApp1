#pragma once
#include<math.h>
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
		if (abs(x) < 0.000001 && abs(y) < 0.000001)return true;
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
	Size2D():width(0),height(0)
	{
		
	}
	int width;
	int height;
};
/*
��Ϸ����������Ļ���
*/
class spirit
{
public:
	spirit()
	{
		
	}
	//��ǰ�ٶ� ��λ��������/s��
	float speed = 0;
	//����ٶ�
	float MaxSpeed = 0;
	//���ٶ�
	float acceleration = 0;
	float x=0;
	float y=0;
	//����
	vector2D directionvector;
	//�����С
	Size2D size;
	//�����˶�
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