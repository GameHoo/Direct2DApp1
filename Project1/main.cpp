#include<iostream>
#include<stdio.h>
#include"HGWindow.h"
#include "HGDirect2D.h"
#include "HGInput.h"
#include"GameTimer.hpp"
#include "Spirit.hpp".
#include "Player.hpp"
#include"Bullet1.hpp"
#include "Enemy1.hpp"
#include<Windows.h>
#include <sstream>
#include <vector>
#include<random>
using namespace std;


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
//是否到时间生成敌机1
bool isTimeToEnemy1(float deltatime);
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
bool isRun = true;
random_device rd;
mt19937 mt(rd());
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
	
	Player* player = static_cast<Player*>(Spirit_List[0]);
	//获取用户输入
	int keydown = 0;
	if(theInput.isKeyDown(DIK_UPARROW))
	{
		keydown += 1;
	}
	if(theInput.isKeyDown(DIK_DOWNARROW))
	{
		keydown += 2;	
	}
	if (theInput.isKeyDown(DIK_LEFTARROW))
	{
		keydown += 4;	
	}
	if (theInput.isKeyDown(DIK_RIGHTARROW))
	{
		keydown += 8;
	}
	//精灵活动
	
	for (vector<Spirit*>::iterator i=Spirit_List.begin(); i !=Spirit_List.end();)
	{
		Spirit* temp = *i;
		if(temp->id==BMP_ID::PLAYER)
		{
		static_cast<Player*>(temp)->action(Spirit_List,i,theTimer.DeltaTime(),keydown);
		//PLAYER会发射子弹		
		}
		else if(temp->id==BMP_ID::BULLET_1)
		{
			static_cast<Bullet1*>(temp)->action(Spirit_List,theTimer.DeltaTime());	
			i++;
		}
		else if (temp->id == BMP_ID::ENEMY_1)
		{
			static_cast<Enemy1*>(temp)->action(Spirit_List,i,theTimer.DeltaTime());
		}
		else
		{
			i++;
		}
	}

	
	wostringstream outs;
	outs << L"x:" << player->x << L" y:" << player->y;
	SetWindowText(theWindow.getHwnd(), outs.str().c_str());
	//生成敌机
	
	if(isTimeToEnemy1(theTimer.DeltaTime()))
	{
		
		int temp = mt();
		temp %= 600;
		Spirit_List.push_back(new Enemy1(temp,0.f));

	}
	//删除HP=0的精灵
	for (vector<Spirit*>::iterator i = Spirit_List.begin(); i != Spirit_List.end();)
	{
		Spirit* temp = *i;
		if(temp->hp<=0)
		{
			if(temp->id==BMP_ID::PLAYER)//游戏结束
			{
				i++;
			}
			else
			{
				delete temp;
				i = Spirit_List.erase(i);
			}
			
		}
		else
		{
			i++;
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
//是否到时间生成敌机1
bool isTimeToEnemy1(float deltatime)
{
	//累计时间
	static float time = 5.f;
	if (time >= 5.f)
	{
		time = 0;
		return true;
	}
	else
	{
		time += deltatime;
		return false;
	}
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
		//RenderTarget->BeginDraw();
		theDirect2D.DrawSprit(temp);
		//RenderTarget->EndDraw();
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
