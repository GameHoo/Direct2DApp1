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
struct BMP_Info
{
	IWICFormatConverter* Converter;
	int width;
	int height;
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

public:
	int hp;
	sprite()
	{
		hp = 1000;
		CantBeAttack = true;
	}
	~sprite() {}
	
	virtual void action(vector<sprite*>&sprite_List,float DeltaTime,int keydown)
	{
		move(DeltaTime);
	}
	 void LoadResource()
	{
		if (Resource.find(id) != Resource.end())
		{
			width = Resource[id].width;
			height = Resource[id].height;
			return;
		}
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
		Resource[id].width = width;
		Resource[id].height = height;
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
		Resource[id].Converter = pConverter;
		WICFactory->Release();
		pDecoder->Release();
		pSource->Release();
		//pStream->Release();
		pScaler->Release();
	}
	static map<wstring, BMP_Info>& Resource;
	bool CantBeAttack;
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

};
class player:public sprite
{
public:
	//飞机发射子弹频率 单位：   个/秒 
	float ShootRate = 20;
	//飞机一次发射子弹数量 1-5个
	int ShootNumber = 5;
	bool Have_Follow_bullet = true;
	player()
	{
		x = 300; y = 500;
		MaxSpeed = 300;
		acceleration = 180000;
		attack = 1000;
		Is_Enemy = false;
		id = L"player";		
		CantBeAttack = false;
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
	bullet() {}
	~bullet() {}


	
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
class Tracking_bullet:public sprite
{
public:
	void action(vector<sprite*>&sprite_List, float DeltaTime, int keydown)
	{
		int nearest_position_x ;
		int nearest_position_y;
		int nearest_position_distace = INT_MAX;
		for (vector<sprite*>::iterator i = sprite_List.begin()+1; i != sprite_List.end();i++)
		{
			sprite* temp = *i;
			if (!temp->CantBeAttack)
			{
				if (temp->Is_Enemy != Is_Enemy)
				{
					int distance = (temp->x - x)*(temp->x - x) + (temp->y - y)*(temp->y - y);
					if (distance < nearest_position_distace)
					{
						nearest_position_x = temp->x;
						nearest_position_y = temp->y;
						nearest_position_distace = distance;
					}
				}
			}
		}
		if(nearest_position_distace!=INT_MAX)
		{
			direction = vector2D(nearest_position_x - x, nearest_position_y - y);
			direction.VectorToOne();
		}
		sprite::action(sprite_List, DeltaTime, keydown);
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
class bulletFactory
{
public:
	bulletFactory(){}
	~bulletFactory(){}
	static sprite* CreatBullet(wstring _id, vector2D _direction, float _x, float _y, bool _IsEnemy = false)
	{
		sprite* the = nullptr;
	
		if (_id == L"bullet1")//普通子弹
		{
			the = new bullet();
			the->id = _id;
			the->MaxSpeed = 500;
			the->acceleration = 100000;
			the->attack = 16;
			the->direction = _direction;
			the->Is_Enemy = _IsEnemy;
			the->x = _x;
			the->y = _y;
			the->CantBeAttack = false;
		}
		else if (_id == L"bullet2")//大圆子弹 速度慢 威力大
		{
			the = new bullet();
			the->id = _id;
			the->MaxSpeed = 100;
			the->acceleration = 10000;
			the->attack = 300;
			the->direction = _direction;
			the->Is_Enemy = _IsEnemy;
			the->x = _x;
			the->y = _y;
			the->CantBeAttack = true;
		}
		else if (_id == L"bullet3")//跟踪子弹 速度慢 威力大
		{
			the = new Tracking_bullet();
			the->id = _id;
			the->MaxSpeed = 1500;
			the->acceleration = 15000;
			the->attack = 100;
			the->direction = _direction;
			the->Is_Enemy = _IsEnemy;
			the->x = _x;
			the->y = _y;
			the->CantBeAttack = false;
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
		CantBeAttack = false;
		LoadResource();
	}	
	~Enemy1(){}
	bool shoot(Shoot_Info&info, float deltatime)
	{
		if (isTimeToShoot(deltatime))
		{
			info.Have_FollowBullet=false;
			info.Shoot_number = 5;
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
class Enemy2 :public sprite
{
public:
	static float anchor_x;
	static float anchor_y;
	static float init_x;
	static float init_y;
	static bool isclockwise;
	Enemy2()
	{
		id = L"enemy2";
		LoadResource();
		CantBeAttack = false;
		Is_Enemy = true;
	}
	//第一阶段 从(300,0)往左或者往右画圆 第二阶段 接着往右或者往左
	static void GroupAction(vector<sprite*>& sprite_List, float deltatime)
	{
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_int_distribution<> dis(0, 1);
		static int Number_Of_Create = 5;
		static sprite* last = nullptr;	
		static int stage = 1;
		if(stage==1)//第一阶段
		{
			if(Number_Of_Create==5)//第一阶段初始
			{
				if (!isTimeToCreate_Group(deltatime))return;
				isclockwise = dis(mt);
				if(isclockwise)//left
				{
					anchor_x = 0;
				}
				else
				{
					anchor_x = 600;
				}
				init_x = 300; init_y = 0;
				anchor_y = 0;		
			}
			if (Number_Of_Create == 0)//第一阶段 等待结束
			{
				if(last->hp<=0)//第一阶段结束
				{
					Number_Of_Create = 5;
					last = nullptr;
					stage = 2;
				}
				return;
			}
			if(isTimeToCreate_One(deltatime))//第一阶段进行
			{
				sprite* temp = new Enemy2();
				temp->x = init_x; temp->y = init_y;
				sprite_List.push_back(temp);
				Number_Of_Create--;
				if (Number_Of_Create == 0)
					last = temp;
			}
		}
		else if(stage==2)//第二阶段
		{
 			if (Number_Of_Create == 5)//第二阶段初始
			{
				if (isclockwise)//left
				{
					anchor_x = 0;
					init_x = 0;
				}
				else
				{
					init_x = 600;
					anchor_x= 600;
				}
				init_y = 300;
				anchor_y = 600;
			}
			if(Number_Of_Create==0)//第二阶段等待
			{
				if (last->hp <= 0)//第二阶段结束
				{
					Number_Of_Create = 5;
					stage = 1;
					last = nullptr;
				}
				return;
			}
			if (isTimeToCreate_One(deltatime))//第二阶段进行
			{
				sprite* temp = new Enemy2();
				temp->x = init_x; temp->y = init_y;
				sprite_List.push_back(temp);
				Number_Of_Create--;
				if (Number_Of_Create == 0)
					last = temp;
			}
		}

	}
	static bool isTimeToCreate_One(float deltatime)
	{
		float time = 0.2f;
		static float time_interval = time;
		if (time_interval >= time)
		{
			time_interval = 0;
			return true;
		}
		else
		{
			time_interval += deltatime;
			return false;
		}
	}
	static bool isTimeToCreate_Group(float deltatime)
	{
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<> dis(0, 20);
		float time = dis(mt);//dis(mt);
		static	float time_interval=time;
		if (time_interval >= time)
		{
			time_interval = 0;
			return true;
		}
		else
		{
			time_interval += deltatime;
			return false;
		}
	}
	void action(vector<sprite*>&sprite_List, float DeltaTime, int keydown)
	{
		vector2D temp = vector2D(x - anchor_x, y - anchor_y);
		temp.VectorToOne();
		float angle = temp.VecoterToAngle();
		float angle_add = DeltaTime*(180/3.1415926);
		if (!isclockwise)
			angle_add = -angle_add;
		float speed_angle;
		angle = fmodf((angle + angle_add + 359), 359);
		speed_angle = fmodf(angle + 90,359);
		if(!isclockwise)
			speed_angle = fmodf(angle - 90, 359);
		temp = vector2D::AngleToVector(angle);
		temp*300;
		x = temp.x + anchor_x;
		y = temp.y + anchor_y;
		speed = vector2D::AngleToVector(speed_angle);

		float left = 0.f - width;
		float right = 600.f + width;
		float top = 0.f - height;
		float bottom = 600.f + height;
		if (x < left)
			hp = 0;
		if (x > right)
			hp = 0;
		if (y < top) 
			hp = 0;
		if (y > bottom)
			hp = 0;
	}

};
class cloud:sprite
{
public:

	void action(vector<sprite*>&sprite_List, float DeltaTime, int keydown)
	{
	
		sprite::action(sprite_List, DeltaTime, keydown);
		float left = 0.f - width;
		float right = 600.f + width;
		float top = 0.f - height;
		float bottom = 600.f + height;
		if (x < left)
			hp = 0;
		if (x > right)
			hp = 0;
		if (y < top)
			hp = 0;
		if (y > bottom)
			hp = 0;
	}
	static bool isTimeToCreate_One(float deltatime)
	{
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<> dis(0, 2.5);
		static float time = dis(mt);
		static float time_interval = time;
		if (time_interval >= time)
		{
			time_interval = 0;
			time = dis(mt);
			return true;
		}
		else
		{
			time_interval += deltatime;
			return false;
		}
	}
	static void GroupAction(vector<sprite*>& sprite_List, float deltatime)
	{
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<> dis(0.2, 0.5);
		std::uniform_int_distribution<> dis2(0, 600);
		if (!isTimeToCreate_One(deltatime))return;
		float scaling_factor = dis(mt);
		sprite* temp = new cloud();
		temp->speed;
		temp->MaxSpeed = 300;
		temp->direction = vector2D(0, 1);
		temp->acceleration = 10000;
		temp->id = L"cloud";
		temp->y = 0; temp->x = dis2(mt);
		temp->LoadResource();
		temp->width *=scaling_factor;
		temp->height *=scaling_factor;
		sprite_List.push_back(temp);
	}
};