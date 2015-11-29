#pragma once
#include<map>
#include<string>
#include<d2d1.h>
#pragma comment(lib,"d2d1.lib")
#include<wincodec.h>
#pragma comment(lib, "Windowscodecs.lib" )
#include<vector>
using namespace std;

class vector2D
{
public:
	vector2D(float ax = 0, float ay = 0)
	{
		x = ax; y = ay;
	}
	float VecoterToAngle()
	{
		float angle = 0;
		if (!this->isZero())
		{
			this->VectorToOne();
			angle = acos(-1 * this->y)*(180 / 3.1415926);
			if (this->x < 0)
			{
				angle = 360.f - angle;
			}
		}
		return angle;
	}
	static vector2D AngleToVector(float angle)
	{
		float x =  sin(angle *(3.1415926/180.f));
		float y =  cos(angle *(3.1415926/180.f));
		y = -y;
		
		vector2D the = vector2D(x, y);
		the.VectorToOne();
		return the;
	}
	float x; float y;
	bool isZero()
	{
		if (fabsf(x) < 0.000001 && fabsf(y) < 0.000001)return true;
		return false;
	}
	float GetModel()
	{
		return sqrt(x*x + y*y);
	}
	void VectorToOne()
	{
		float Model = GetModel();
		x /= Model;
		y /= Model;
	}
	vector2D operator*(float s)
	{
		vector2D temp;
		temp.x = x*s;
		temp.y = y*s;
		return temp;
	}

};

class spirit
{
protected:
	int hp;
public:
	spirit()
	{
		hp = 1000;
	}
	~spirit() {}
	virtual void action(float DeltaTime,int keydown)
	{
		move(DeltaTime);
	}
	void getAttack(int attack)
	{
		if(!Is_NoAttack)
		{
			hp -= attack;
		}
	}
	 void LoadResource()
	{
		IWICImagingFactory* WICFactory;
		IWICBitmapDecoder *pDecoder = NULL;
		IWICBitmapFrameDecode *pSource = NULL;
		//IWICStream *pStream = NULL;
		IWICFormatConverter *pConverter = NULL;
		IWICBitmapScaler *pScaler = NULL;
		wstring FileName = L"file/" + id + L".png";
		CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&WICFactory)
			);
		WICFactory->CreateDecoderFromFilename(FileName.c_str(),
			0, GENERIC_READ, WICDecodeMetadataCacheOnLoad,
			&pDecoder);
		pDecoder->GetFrame(0, &pSource);
		UINT originalWidth, originalHeight;
		pSource->GetSize(&originalWidth, &originalHeight);
		width = originalWidth; height = originalHeight;
		WICFactory->CreateBitmapScaler(&pScaler);
		pScaler->Initialize(
			pSource,
			originalWidth,
			originalHeight,
			WICBitmapInterpolationModeCubic
			);
		WICFactory->CreateFormatConverter(&pConverter);
		pConverter->Initialize(
			pScaler,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.0,
			WICBitmapPaletteTypeMedianCut
			);
		Resource[id] = pConverter;
		WICFactory->Release();
		pDecoder->Release();
		pSource->Release();
		//pStream->Release();
		pScaler->Release();
	}
	static map<wstring, IWICFormatConverter*>& Resource;
	bool Is_NoAttack;
	bool Is_Enemy;
	vector2D speed;
	float MaxSpeed;
	float acceleration;
	float x;
	float y;
	vector2D direction;
	float width;
	float height;
	wstring id;	
	int attack;
	virtual	float getAngle()
	{
		return speed.VecoterToAngle();
	}
	virtual bool shoot(wstring& bullet_ID,int& bullet_Number, bool &bullet_IsEnemy,float deltatime)
	{
		return false;
	}
	bool IsEnemy(spirit* target)
	{
		return target->Is_Enemy != Is_Enemy;
	}
	bool IsDead()
	{
		if (hp <= 0)
		{
			return true;
		}
		return false;
	}

	void move(float DeltaTime)
	{
		if (direction.isZero())
		{
			return;
		}
		else
		{
			direction.VectorToOne();
			direction = direction*(acceleration*DeltaTime);
			speed.x += direction.x;
			speed.y += direction.y;
			if (speed.GetModel() > MaxSpeed)
			{
				speed.VectorToOne();
				speed = speed*MaxSpeed;
			}
			if (speed.isZero())
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
	bool attackTest(spirit* target)
	{
		if ((target->width / 2 + width / 2) > fabs(target->x - x) && (target->height / 2 + height / 2) > fabs(target->y - y))
		{
			return true;
		}
		return false;
	}
};
class player:public spirit
{
public:
	//飞机发射子弹频率 单位：   个/秒 
	float ShootRate = 2;
	//飞机一次发射子弹数量 1-5个
	int ShootNumber = 19;
	player()
	{
		x = 300; y = 500;
		MaxSpeed = 300;
		acceleration = 180000;
		attack = 1000;
		Is_Enemy = false;
		id = L"player";		
		LoadResource();
	}
	~player(){}
	/*
	float getAngle()
	{
		return 0.f;
	}*/
	bool isTimeToShoot(float deltatime)
	{
		static float time = 1.0/ ShootRate;
		if(time>=1.0/ShootRate)
		{
			time = 0; return true;
		}
		else
		{
			time += deltatime; return false;
		}
	}
	void action(float DeltaTime, int keydown)
	{
		direction = vector2D(0, 0);
		if (keydown & 1)
		{
			direction.y += -1;
		}
		if (keydown & 2)
		{
			direction.y += 1;
		}
		if (keydown & 4)
		{
			direction.x += -1;
		}
		if (keydown & 8)
		{
			direction.x += 1;
		}
		spirit::action( DeltaTime,keydown);
		//移动范围限制
		float left = 0.f + width / 2;
		float right = 600.f - width / 2;
		float top = 0.f + height / 2;
		float bottom = 600.f - height / 2;
		if (x < left)x = left;
		if (x > right)x = right;
		if (y < top) y = top;
		if (y > bottom)y = bottom;
		
	}
	virtual bool shoot(wstring& bullet_ID, int& bullet_Number, bool &bullet_IsEnemy, float deltatime)
	{
		if(isTimeToShoot(deltatime))
		{
			bullet_ID = L"bullet1";
			bullet_Number = ShootNumber;
			bullet_IsEnemy = Is_Enemy;
			return true;
		}
		return false;
	}
};
class bullet:public spirit
{
private:
	bullet() {}
public:
	~bullet() {}
	static bullet* CreatBullet(wstring _id,vector2D _direction,float _x,float _y,bool _IsEnemy=false)
	{
		bullet* the = nullptr;
		the = new bullet();
		the->id = _id;
		if(the->id==L"bullet1")
		{
			
			the->MaxSpeed = 500;
			the->acceleration = 10000;
			the->attack = 100;
			the->direction = _direction;
			the->Is_Enemy = _IsEnemy;
			the-> x= _x;
			the->y = _y;
		}
		else if (the->id == L"bullet2")
		{ 

		}
		the->LoadResource();
		return the;
	}

	
	void action(float DeltaTime,int keydown)
	{
		spirit::action(DeltaTime,keydown);
		//移动范围限制
		float left = 0.f - width ;
		float right = 600.f + width;
		float top = 0.f - height;
		float bottom = 600.f + height;
		if (x < left)hp = 0;
		if (x > right)hp = 0;
		if (y < top) hp = 0;
		if (y > bottom)hp = 0;
	}
};
class Enemy1:public spirit
{
public:
	Enemy1(){}	
	~Enemy1(){}
	void action(float DeltaTime, int keydown)
	{
		spirit::action(DeltaTime, keydown);
		//移动范围限制
		float left = 0.f - width;
		float right = 600.f + width;
		float top = 0.f - height;
		float bottom = 600.f + height;
		if (x < left)hp = 0;
		if (x > right)hp = 0;
		if (y < top) hp = 0;
		if (y > bottom)hp = 0;
	}
};