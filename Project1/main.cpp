#include<iostream>
#include<stdio.h>
#include"HGWindow.h"
#include "HGDirect2D.h"
#include "HGInput.h"
#include"GameTimer.hpp"
#include "Spirit.hpp".
#include "Player.hpp"
#include<Windows.h>
#include <sstream>
#include <crtdbg.h>
#include <vector>
using namespace std;
int isOutOfRange(float x, float y, float width, float height);
bool Forceoffset(vector2D &direction, int result);
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
	int result = isOutOfRange(Spirit_List[0]->x, Spirit_List[0]->y, Spirit_List[0]->size.width, Spirit_List[0]->size.height);
	Forceoffset(direction, result);
	//飞机运动
	Spirit_List[0]->directionvector = direction;
	Spirit_List[0]->move(theTimer.DeltaTime());
}
/*
判断物体是否在界限上
返回0不越界
    1左边界限
    2上
    4右
    8下
*/
//根据OutOfRange返回值抵消里 返回值为ture代表产生了抵消
bool Forceoffset(vector2D &direction,int result)
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
int isOutOfRange(float x,float y,float width,float height)
{
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
	CalculateFPS();
	ID2D1RenderTarget* RenderTarget = static_cast<ID2D1RenderTarget*>(theDirect2D.Get_RenderTarget());
	ID2D1SolidColorBrush *brush1;
	RECT rc;
	GetClientRect(theWindow.getHwnd(), &rc);

	D2D1_RECT_F layoutRect = D2D1::RectF(
		static_cast<FLOAT>(rc.left),
		static_cast<FLOAT>(rc.top),
		static_cast<FLOAT>(rc.right - rc.left),
		static_cast<FLOAT>(rc.bottom - rc.top)
		);
	RenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
		&brush1
		);

	RenderTarget->BeginDraw();

	RenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	RenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));

	theDirect2D.DrawSprit(Spirit_List[0]);
	
	
	
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
	
		//Render();
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
