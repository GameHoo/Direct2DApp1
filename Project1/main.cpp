#include<iostream>
#include<stdio.h>
#include"HGWindow.h"
#include "HGDirect2D.h"
#include "HGInput.h"
#include"GameTimer.hpp"
#include "Spirit.hpp".
#include "Player.hpp"
#include"Bullet1.hpp"
#include<Windows.h>
#include <sstream>
#include <crtdbg.h>
#include <vector>
using namespace std;
int isOutOfRange(Spirit* spirit);
bool Forceoffset(vector2D &direction,
	int result
	);
void CorrectSpiritPosition(Spirit* s);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void CalculateFPS();
void Render();
void Update(float Delta);
int width = 800;
int height = 600;
HGWindow theWindow;
HGDirect2D theDirect2D;
HGInput theInput;
GameTimer theTimer;
//精灵列表
vector<Spirit*> Spirit_List;
//敌机数量
int Number_Of_Enemy = 0;
//我方子弹导弹数量
int Number_Of_Ours = 0;
//敌方子弹导弹数量
int Number_Of_Theirs = 0;

bool isRun = true;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
    
	theWindow.HG_Init_Window(L"雷霆战机", WndProc, hInstance, nCmdShow, width, height);
	theDirect2D.Init(theWindow.getHwnd());
	theInput.init(hInstance, theWindow.getHwnd());
	theTimer.Reset();
	//生成玩家
	Spirit_List.push_back(new Player());

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
	//_CrtDumpMemoryLeaks();
	return 0;
}
//逻辑计算
void Update(float Delta)
{
	//产生牵扯力
	vector2D direction=vector2D(0,0);
	//这次逻辑结束需要删除的精灵
	vector<Spirit*>Delete_List;
	Player* player = static_cast<Player*>(Spirit_List[0]);

	if(theInput.isKeyDown(DIK_UPARROW))
	{
		direction.y += -1;
		
	}
	if(theInput.isKeyDown(DIK_DOWNARROW))
	{
		direction.y += 1;
		
	}
	if (theInput.isKeyDown(DIK_LEFTARROW))
	{
		direction.x += -1;
		
	}
	if (theInput.isKeyDown(DIK_RIGHTARROW))
	{
		direction.x += 1;
		
	}

	
	
	//在窗口界限抵消对应的力
	int result = isOutOfRange(player);
	Forceoffset(direction, result);
	//飞机运动
	player->directionvector = direction;
	player->move(theTimer.DeltaTime());
	//最后因为deltatime太小 物体速度很大 还是会越界 修正坐标
	CorrectSpiritPosition(player);
	//其余精灵移动
	for (vector<Spirit*>::iterator i=Spirit_List.begin()+1; i!=Spirit_List.end(); i++)
	{
		Spirit* temp = *i;
		temp->move(theTimer.DeltaTime());
	}
	//检查现有子弹是否越界
	for (int i = 1; i <= Number_Of_Ours; i++)
	{
		//子弹越界消失
		Spirit* temp = Spirit_List[i];
		if (isOutOfRange(temp))
		{
			Delete_List.push_back(temp);
		}
	}
	//生成我方子弹
	if (player->ShootNumber == 1) {
		if (player->isTimeToShoot(theTimer.DeltaTime()))
		{
			Bullet1* bullet = new Bullet1();
			bullet->x = player->x;
			bullet->y = player->y - player->size.height;
			Number_Of_Ours++;
			Spirit_List.insert(Spirit_List.begin() + Number_Of_Ours, bullet);
		}
	}
	wostringstream outs;
	outs << "x:" << Spirit_List[0]->x
		<< " y:" << Spirit_List[0]->y;
	SetWindowText(theWindow.getHwnd(), outs.str().c_str());
	//删除需要删除的精灵
	for (vector<Spirit*>::iterator i=Delete_List.begin(); i != Delete_List.end(); i++)
	{
		Spirit* temp = *i;
		for (vector<Spirit*>::iterator j = Spirit_List.begin(); j != Spirit_List.end(); j++)
		{
			if (temp == *j)
			{
				/*wostringstream outs;
				outs << L"speed:" << (temp->speed.GetModel())<< L" 加速度" << (temp->acceleration);
				SetWindowText(theWindow.getHwnd(), outs.str().c_str());
				j=Spirit_List.erase(j);*/
				if (temp->id == BMP_ID::BULLET_1)
				{
					Number_Of_Ours--;
				}
				break;
			}
		}
		
	}
}
/*
判断物体是否在界限上
返回0不越界
    1左边界限
    2上
    4右
    8下
*/
//修正精灵位置使其在游戏区
void CorrectSpiritPosition(Spirit* s)
{
	float left = 0.f + s->size.width / 2;
	float right = 600.f - s->size.width / 2;
	float top = 0.f + s->size.height / 2;
	float bottom = 600.f - s->size.height / 2;
	if (s->x < left)s->x = left;
	if (s->x > right)s->x = right;
	if (s->y < top)s->y=top;
	if (s->y > bottom)s->y = bottom;
}
//撞墙抵消力
bool Forceoffset(vector2D &direction,
	int result
	)
{
	bool r = false;
	//在左边界 抵消向左的力
	if (result & 1)
	{
		if (direction.x < 0)
		{
			direction.x = 0;
			r = true;
			
		}
	}
	if (result & 2)
	{
		if (direction.y < 0)
		{
			direction.y = 0;
			r = true;
			
		}
	}
	if (result & 4)
	{
		if (direction.x > 0)
		{
			direction.x = 0;
			r = true;
			
		}
	}
	if (result & 8)
	{
		if (direction.y > 0)
		{
			direction.y = 0;
			r = true;
			
		}

	}
	return r;
}
//求精灵是否越界
int isOutOfRange(Spirit* spirit)
{
	int x = spirit->x;
	int y = spirit->y;
	int width = spirit->size.width;
	int height = spirit->size.height;
	int result = 0;
	if (x < 0.f + width/2)result+=1;
	if (y < 0.f + height/2)result+= 2;
	if (x > 0.f + 600.f-width/2)result += 4;
	if (y > 0.f + 600.f-height/2)result += 8;
	return result;
}
//画面渲染
void Render()
{
	
	ID2D1RenderTarget* RenderTarget = static_cast<ID2D1RenderTarget*>(theDirect2D.Get_RenderTarget());
	ID2D1SolidColorBrush *brush1;

	RenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF(0xececee)),
		&brush1
		);

	RenderTarget->BeginDraw();
	RenderTarget->Clear(D2D1::ColorF(D2D1::ColorF(0xacd0ce)));
	RenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	//绘制菜单
	RenderTarget->DrawLine(D2D1::Point2F(700.f, 0.f),
		D2D1::Point2F(700.f, 600.f), brush1, 200.f);
	
	//画出每个精灵
	for (vector<Spirit*>::iterator i = Spirit_List.begin(); i != Spirit_List.end(); i++)
	{
		Spirit* temp = *i;
		theDirect2D.DrawSprit(temp);
	}
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
		SetWindowText(theWindow.getHwnd(), outs.str().c_str());
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
 	   ValidateRect(theWindow.getHwnd(), nullptr);
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
