#pragma once
#include<math.h>
//��ʾ2Dʸ��
class vector2D
{
public:
	float x;
	float y;
	//������ģ
	float GetModel()
	{
		return sqrt(x*x + y*y);
	}
	//������һ�� ת���ɵ�λ����
	void VectorToOne()
	{
		float Model = GetModel();
		if (Model < 0.0001)return;//����С���� ����
		x /= Model;
		y /= Model;
	}
	//��������
	vector2D operator+(float s)
	{
		vector2D temp;
		temp.x = x*s;
		temp.y = y*s;
		return temp;
	}
};
/*
��Ϸ����������Ļ���
*/
class Spirit
{

protected:
	//��ǰ�ٶ� ��λ��������/s��
	float speed=0;
	//����ٶ�
	float MaxSpeed = 0;
	//���ٶ�
	float acceleration = 0;
public:
	float x=0;
	float y=0;
	//�����˶�
	void move()
	{
		
	}
};