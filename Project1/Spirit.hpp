#pragma once
#include<math.h>
enum BMP_ID
{
	PLAYER= 0,
	BULLET_1=1

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
	//��ǰ�ٶ� ��λ��������/s��
	vector2D speed ;
	//����ٶ�
	float MaxSpeed;
	//���ٶ�
	float acceleration;
	float x;
	float y;
	//����
	vector2D directionvector;
	//�����С
	Size2D size;
	//ͼƬ��ԴID
	BMP_ID id;
	//�õ��˶������Y�������������ļн�
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
	//�����˶�
	void move(float DeltaTime)
	{
		if(directionvector.isZero())//û�з���
		{
			return;
		}
		else
		{
			//�������ķ��� �����x y ���ٶȷ���
			directionvector.VectorToOne();
			directionvector = directionvector*(acceleration*DeltaTime);

			//���ٶȷ����ӵ��ٶ���
			speed.x += directionvector.x;
			speed.y += directionvector.y;
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