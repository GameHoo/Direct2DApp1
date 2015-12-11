#include<Windows.h>
#include"support.hpp"
#include"GameTimer.hpp"
#include "sprites.hpp"
#include <sstream>
#include <vector>
#include<random>
#include<map>
using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////
//函数声明
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void shoot(sprite* the, Shoot_Info info, vector<sprite*>::iterator& i);
void attackTest(sprite* target1, sprite* target2);
void CalculateFPS();
void Render();
void Update(float Delta);
void DrawSprit(sprite* the);
void init(HINSTANCE hInstance, int nCmdShow);
/////////////////////////////////////////////////////////////////////////////////////////
//全局变量
HWND hwnd;
int width = 800;
int height = 600;
sprite* player1;
HGInput theInput;
GameTimer theTimer;
vector<sprite*> sprite_List;
bool isRun = true;
ID2D1RenderTarget* RenderTarget;
map<wstring, BMP_Info> temp;
map<wstring, BMP_Info> &sprite::Resource = temp;
float Enemy2::anchor_x;
float Enemy2::anchor_y;
float Enemy2::init_x;
float Enemy2::init_y;
bool Enemy2::isclockwise;

//////////////////////////////////////////////////////////////////////////////////////////
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	init(hInstance,nCmdShow);
	//生成玩家
	player1= new player();
	sprite_List.push_back(player1);
	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			DispatchMessage(&msg);
		}
		if (isRun)
		{
			//执行逻辑
			Update(theTimer.DeltaTime());
			Render();
			theTimer.Tick();
		}
	}
	return 0;
}
//逻辑计算
void Update(float deltatime)
{
	CalculateFPS();
	//获取用户输入
	int keydown = 0;
	theInput.Keyboards_Read();
	if(theInput.keyDown(DIK_UPARROW))
	{
		keydown += 1;
	}
	if (theInput.keyDown(DIK_DOWNARROW))
	{
		keydown += 2;	
	}
	if (theInput.keyDown(DIK_LEFTARROW))
	{
		keydown += 4;	
	}
	if (theInput.keyDown(DIK_RIGHTARROW))
	{
		keydown += 8;
	}	
	//attack test
	for (vector<sprite*>::iterator i = sprite_List.begin(); i != sprite_List.end(); i++)
	{
		for (vector<sprite*>::iterator j = i; j != sprite_List.end(); j++)
		{
			attackTest(*i, *j);
		}
	}
	for (vector<sprite*>::iterator i=sprite_List.begin(); i !=sprite_List.end();i=i+1)
	{
		sprite* the = *i;
		the->action(sprite_List, deltatime, keydown);
		Shoot_Info info;
		if(the->shoot(info, deltatime))shoot(the, info,i);
	}
	//Group Action
	Enemy1::GroupAction(sprite_List, theTimer.DeltaTime());
	Enemy2::GroupAction(sprite_List, theTimer.DeltaTime());
	cloud::GroupAction(sprite_List, theTimer.DeltaTime());
	//删除HP=0的精灵
	for (vector<sprite*>::iterator i = sprite_List.begin(); i != sprite_List.end();)
	{
		sprite* temp = *i;
		if(temp->hp<=0)
		{
			
			delete temp;
			i = sprite_List.erase(i);
			temp = nullptr;

		}
		else
		{
			i++;
		}
	}
}
//碰撞检测
void attackTest(sprite* target1,sprite* target2)
{
	if (target1->id == L"bkg1" || target2->id == L"bkg1" || target1->id == L"bkg2" || target2->id == L"bkg2")return;
	if ((target1->width / 2 + target2->width / 2) > fabs(target1->x - target2->x) && (target1->height / 2 + target2->height / 2) > fabs(target1->y - target2->y))
	{
		if(target1->Is_Enemy!=target2->Is_Enemy)
		{
		if (!target1->CantBeAttack)target1->hp -= target2->attack;
		if (!target2->CantBeAttack)target2->hp -= target1->attack; 
		}
	}
}

void Render()
{

	
	ID2D1SolidColorBrush *brush1;
	RenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF(0xececee)),
		&brush1
		);
	RenderTarget->BeginDraw();

	RenderTarget->Clear(D2D1::ColorF(D2D1::ColorF(0xeeeeee)));

	RenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	
	
	//画出每个精灵
	for (vector<sprite*>::iterator i = sprite_List.begin(); i != sprite_List.end(); i++)
	{
		sprite* temp = *i;
		//RenderTarget->BeginDraw();
		DrawSprit(temp);
		//RenderTarget->EndDraw();
	}
	//绘制菜单
	RenderTarget->DrawLine(D2D1::Point2F(700.f, 0.f),
	D2D1::Point2F(700.f, 600.f), brush1, 200.f);
	RenderTarget->EndDraw();
}
//计算FPS
void CalculateFPS()
{
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;
	frameCnt++;
	if ((theTimer.TotalTime() - timeElapsed) >= 1.f)//1.0ms刷新一次
	{
		float fps = (float)frameCnt;
		float mspf = 1000.0f / fps;

		wostringstream outs;
		outs.precision(6);
		outs << L"雷霆战机" << L"   "
			<< L"FPS: " << fps << L"   "
			<< L"Frame Time:" << mspf << L"(ms)";
		SetWindowText(hwnd, outs.str().c_str());
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}
//窗口循环
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_ACTIVATE:
	{
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			theTimer.Stop();
			isRun = false;
		}
		else
		{
			theTimer.Start();
			isRun = true;
		}
	}
	break;
	case WM_PAINT:
	{
 	   ValidateRect(hwnd, nullptr);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_MOVE:
		//Render();
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);

	}
	return 0;
}
void DrawSprit(sprite* the)
{
	static map<IWICFormatConverter*, ID2D1Bitmap*> BMP_STORE;
	ID2D1Bitmap* pbitmap;
	int width = the->width;
	int height = the->height;
	float x = the->x;
	float y = the->y;
	float angle = the->getAngle();
	if(BMP_STORE.find(sprite::Resource[the->id].Converter)==BMP_STORE.end())
	{
		RenderTarget->CreateBitmapFromWicBitmap(sprite::Resource[the->id].Converter, nullptr, &pbitmap);
		BMP_STORE[sprite::Resource[the->id].Converter] = pbitmap;
	}
	else
	{
		pbitmap = BMP_STORE[sprite::Resource[the->id].Converter];
	}
	RenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(
		angle,
		D2D1::Point2F(x, y)
		));
	RenderTarget->DrawBitmap(
		pbitmap,
		D2D1::RectF(
			x - width / 2,
			y - height / 2,
			x + width / 2,
			y + height / 2
			));
	RenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());}
void init(HINSTANCE hInstance,int nCmdShow)
{
	hwnd = CreateMyWindow(hInstance, WndProc, nCmdShow, L"雷霆战机", width, height);
	theInput.init(hInstance, hwnd);
	theTimer.Reset();
	RenderTarget = CreateRenderTarget(hwnd);
}
void shoot(sprite* the, Shoot_Info info,vector<sprite*>::iterator& i)
{
	if (info.Shoot_Style == 0)//扇形
	{
		if (info.Shoot_number& 1) //奇数 中间发射子弹
		{
			sprite* a_bullet = bulletFactory::CreatBullet(L"bullet2", vector2D::AngleToVector(the->getAngle()), the->x, the->y, the->Is_Enemy);
			i=sprite_List.insert(i+1, a_bullet);
			//i++;
			info.Shoot_number--;
		}
		int radius = 25;
		int angle_add = 10;
		int left_or_Right = 0;//0 left 1 right
		int add = angle_add;
		while (info.Shoot_number)
		{
			float bullet_angle;//right
			{
				bullet_angle = the->getAngle() + add;
				bullet_angle = (int)(bullet_angle + 360) % 360;
				vector2D direction = vector2D::AngleToVector(bullet_angle);
				direction.VectorToOne();
				i = sprite_List.insert(i+1, bulletFactory::CreatBullet(L"bullet2", direction, the->x+direction.x, the->y+direction.y, the->Is_Enemy));
				//i++;
			}
			{//left
				bullet_angle = the->getAngle() - add;
				bullet_angle = (int)(bullet_angle + 360) % 360;
				vector2D direction = vector2D::AngleToVector(bullet_angle);
				direction.VectorToOne();
				i = sprite_List.insert(i+1, bulletFactory::CreatBullet(L"bullet2", direction, the->x + direction.x, the->y + direction.y, the->Is_Enemy));
				//i++;
			}

			info.Shoot_number = info.Shoot_number - 2;
			add += angle_add;
		}
	}
	else//平行
	{
		int bullet_high = -5;
		int bullet_high_add = 7;
		int bullet_blanking_add = 10;
		int bullet_blanking=10;
		int radius = 30;
		if (info.Shoot_number & 1) //奇数 中间发射子弹
		{
			sprite* a_bullet = bulletFactory::CreatBullet(L"bullet1", vector2D::AngleToVector(the->getAngle()), the->x, the->y -radius + bullet_high, the->Is_Enemy);
			i = sprite_List.insert(i + 1, a_bullet);
			info.Shoot_number--;
			bullet_high += bullet_high_add;
		}
		while (info.Shoot_number)
		{
			//left
			sprite* a_bullet = bulletFactory::CreatBullet(L"bullet1", vector2D::AngleToVector(the->getAngle()), the->x-bullet_blanking, the->y - radius+bullet_high, the->Is_Enemy);
			i = sprite_List.insert(i + 1, a_bullet);
			info.Shoot_number--;
			//right
			a_bullet = bulletFactory::CreatBullet(L"bullet1", vector2D::AngleToVector(the->getAngle()), the->x+bullet_blanking, the->y - radius + bullet_high, the->Is_Enemy);
			i = sprite_List.insert(i + 1, a_bullet);
			info.Shoot_number--;

			bullet_blanking += bullet_blanking_add;
			bullet_high += bullet_high_add;
		}

	}
	if(info.Have_FollowBullet)
	{
		float time = 0.7f;
		static	float interval_time = 0.7f;
		if(interval_time>=time)
		{
			interval_time = 0;
			sprite* a_bullet = bulletFactory::CreatBullet(L"bullet3", vector2D::AngleToVector(the->getAngle()), the->x + 25, the->y - 20, the->Is_Enemy);
			i = sprite_List.insert(i + 1, a_bullet);
			a_bullet = bulletFactory::CreatBullet(L"bullet3", vector2D::AngleToVector(the->getAngle()), the->x - 25, the->y - 20, the->Is_Enemy);
			i = sprite_List.insert(i + 1, a_bullet);
		}
		else
		{
			interval_time += theTimer.DeltaTime();
		}
		
	}
}
