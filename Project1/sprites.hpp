#pragma once
#include<map>
#include<string>
#include<d2d1.h>
#pragma comment(lib,"d2d1.lib")
#include<wincodec.h>
#pragma comment(lib, "Windowscodecs.lib" )
#include<vector>
#include<random>
using namespace std;
struct Shoot_Info
{
	int Shoot_Style;//0扇形 1平行
	int Shoot_number;
	bool Have_FollowBullet;
};
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
	void operator*(float s)
	{
		
		this->x = x*s;
		this->y = y*s;
		
	}

};
class sprite
{
protected:
	int hp;
public:
	sprite()
	{
		hp = 1000;
		Is_NoAttack = true;
	}
	~sprite() {}
	
	virtual void action(vector<sprite*>&sprite_List,float DeltaTime,int keydown)
	{
		move(DeltaTime);
		////碰撞检测
		//for (vector<sprite*>::iterator i = sprite_List.begin(); i != sprite_List.end();i++)
		//{
		//	sprite* target=*i;
		//	if(!target->Is_NoAttack&& IsEnemy(target))
		//	{
		//		if(attackTest(target))
		//		{
		//			target->hp -= attack;
		//		}
		//	}
		//}
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
	virtual bool shoot(Shoot_Info&info,float deltatime)
	{
		return false;
	}
	/*bool IsEnemy(sprite* target)
	{
		return target->Is_Enemy != Is_Enemy;
	}*/
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
			direction*(acceleration*DeltaTime);
			speed.x += direction.x;
			speed.y += direction.y;
			if (speed.GetModel() > MaxSpeed)
			{
				speed.VectorToOne();
				speed*MaxSpeed;
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
	//bool attackTest(sprite* target)
	//{
	//	if ((target->width / 2 + width / 2) > fabs(target->x - x) && (target->height / 2 + height / 2) > fabs(target->y - y))
	//	{
	//		return true;
	//	}
	//	return false;
	//}
};
class player:public sprite
{
public:
	//飞机发射子弹频率 单位：   个/秒 
	float ShootRate = 5;
	//飞机一次发射子弹数量 1-5个
	int ShootNumber = 37;
	bool Have_Follow_bullet = false;
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
	
	float getAngle()
	{
		return 0.f;
	}
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
	void action(vector<sprite*>&sprite_List, float DeltaTime, int keydown)
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
		sprite::action(sprite_List, DeltaTime, keydown);
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
	 bool shoot(Shoot_Info&info, float deltatime)
	{
		if(isTimeToShoot(deltatime))
		{
			info.Have_FollowBullet = Have_Follow_bullet;
			info.Shoot_number = ShootNumber;
			info.Shoot_Style = 1;
			return true;
		}
		return false;
	}
};
class bullet:public sprite
{
private:
	
public:
	//发射起始半径
	float radius;
	bullet() {}
	~bullet() {}
	//static bullet* CreatBullet(wstring _id,vector2D _direction,float _x,float _y,bool _IsEnemy=false)
	//{
	//	bullet* the = nullptr;
	//	the = new bullet();
	//	the->radius = 30.f;
	//	the->id = _id;
	//	if(the->id==L"bullet1")//普通子弹
	//	{
	//		
	//		the->MaxSpeed = 500;
	//		the->acceleration = 10000;
	//		the->attack = 100;
	//		the->direction = _direction;
	//		the->Is_Enemy = _IsEnemy;
	//		the-> x= _x;
	//		the->y = _y;
	//		//根据起始半径修正x y
	//		_direction.VectorToOne();
	//		_direction*the->radius;
	//		the->x += _direction.x;
	//		the->y += _direction.y;
	//	}
	//	else if (the->id == L"bullet2")//大圆子弹 速度慢 威力大
	//	{ 
	//		the->MaxSpeed = 100;
	//		the->acceleration = 10000;
	//		the->attack = 300;
	//		the->direction = _direction;
	//		the->Is_Enemy = _IsEnemy;
	//		the->x = _x;
	//		the->y = _y;
	//		//根据起始半径修正x y
	//		_direction.VectorToOne();
	//		_direction*the->radius;
	//		the->x += _direction.x;
	//		the->y += _direction.y;
	//	}
	//	the->LoadResource();
	//	return the;
	//}

	
	void action(vector<sprite*>&sprite_List, float DeltaTime, int keydown)
	{
		sprite::action(sprite_List, DeltaTime, keydown);
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
class Tracking_bullet
{
	
};
class bulletFactory
{
public:
	bulletFactory(){}
	~bulletFactory(){}
	static sprite* CreatBullet(wstring _id, vector2D _direction, float _x, float _y, bool _IsEnemy = false)
	{
		bullet* the = nullptr;
		the = new bullet();
		the->radius = 30.f;
		the->id = _id;
		if (the->id == L"bullet1")//普通子弹
		{

			the->MaxSpeed = 500;
			the->acceleration = 10000;
			the->attack = 100;
			the->direction = _direction;
			the->Is_Enemy = _IsEnemy;
			the->x = _x;
			the->y = _y;
			//根据起始半径修正x y
			_direction.VectorToOne();
			_direction*the->radius;
			the->x += _direction.x;
			the->y += _direction.y;
			the->Is_NoAttack = false;
		}
		else if (the->id == L"bullet2")//大圆子弹 速度慢 威力大
		{
			the->MaxSpeed = 100;
			the->acceleration = 10000;
			the->attack = 300;
			the->direction = _direction;
			the->Is_Enemy = _IsEnemy;
			the->x = _x;
			the->y = _y;
			//根据起始半径修正x y
			_direction.VectorToOne();
			_direction*the->radius;
			the->x += _direction.x;
			the->y += _direction.y;
			the->Is_NoAttack = true;
		}
		the->LoadResource();
		return the;
	}
};
class Enemy1:public sprite
{
public:
	Enemy1(float _x,float _y)
	{
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_int_distribution<> dis(0, 2);
		id = L"enemy1";
		x = _x; y = _y;
		MaxSpeed = 200;
		acceleration = 400;
		direction = vector2D(dis(mt) - 1,0.5 );
		Is_Enemy = true;
		Is_NoAttack = false;
		LoadResource();
	}	
	~Enemy1(){}
	bool shoot(Shoot_Info&info, float deltatime)
	{
		if (isTimeToShoot(deltatime))
		{
			info.Have_FollowBullet=false;
			info.Shoot_number = 1;
			info.Shoot_Style = 0;
			return true;
		}
		return false;
	}
	bool isTimeToShoot(float deltatime)
	{
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<> dis(0, 10);
		static float random = dis(mt);
		static float time = random;
		if (time >= random)
		{
			random = dis(mt);
			time = 0; 
			return true;
		}
		else
		{
			time += deltatime; 
			return false;
		}
	}
	static void GroupAction(vector<sprite*>& sprite_List,float deltatime)
	{
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_int_distribution<> dis(0, 600);
		if(isTimeToCreate(deltatime))
		{
			sprite_List.push_back(new Enemy1(dis(mt),0));
		}

	}
	float getAngle()
	{
		return 180.f;
	}
	static bool isTimeToCreate(float  deltatime)
	{
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<> dis(0, 5);
		static float random = dis(mt);
		static float time = random;
		if(time>=random)
		{
			time = 0;
			random = dis(mt);
			return true;
		}
		else
		{
			time += deltatime;
			return false;
		}
	}
	void action(vector<sprite*>&sprite_List, float DeltaTime, int keydown)
	{
		sprite::action(sprite_List, DeltaTime, keydown);
		//移动范围限制
		float left = 0.f - width;
		float right = 600.f + width;
		float top = 0.f - height;
		float bottom = 600.f + height;
		if (x < 0+width/2)
		{
			direction.x=5;
			
		}
		if (x > 600-width/2)
		{
			direction.x=-10;
			
		}
		if (y < top) hp = 0;
		if (y > bottom)hp = 0;
	}
};