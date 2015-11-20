#include<iostream>
#include<stdio.h>
#include"HGWindow.h"
#include "HGDirect2D.h"
#include "HGInput.h"
#include"GameTimer.hpp"
#include "Spirit.hpp"
#include<Windows.h>
#include <sstream>
using namespace std;
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
spirit player;


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{

	theWindow.HG_Init_Window(L"雷霆战机", WndProc, hInstance, nCmdShow, width, height);
	theDirect2D.Init(theWindow.getHwnd());
	theInput.init(hInstance, theWindow.getHwnd());
	theTimer.Reset();
	player.x = 400;
	player.y = 600;
	player.speed = 350;
	player.MaxSpeed = 250;
	player.acceleration = 0;

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
		Update(theTimer.DeltaTime());
		Render();
		theTimer.Tick();
	}

	return 0;
}
//逻辑计算
void Update(float Delta)
{
	//wostringstream outs;
	
	
	vector2D direction;
	if(theInput.isKeyDown(DIK_UPARROW))
	{
		direction.x = 0;
		direction.y = -1;
		//outs << L"上";
	}
	else if(theInput.isKeyDown(DIK_DOWNARROW))
	{
		direction.x = 0;
		direction.y = 1;
		//outs << L"下";
	}
	else if (theInput.isKeyDown(DIK_LEFTARROW))
	{
		direction.x = -1;
		direction.y = 0;
		//outs << L"左";
	}
	else if (theInput.isKeyDown(DIK_RIGHTARROW))
	{
		direction.x = 1;
		direction.y = 0;
		//outs << L"右";
	}
	else
	{
		
	}
	player.directionvector = direction;
	//SetWindowText(theWindow.getHwnd(), outs.str().c_str());
	player.move(theTimer.DeltaTime());

}
//渲染
void Render()
{
	//CalculateFPS();
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
	RenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

	theDirect2D.DrawBitmapFromFile(L"C:\\Users\\GameHoo\\Pictures\\雷霆战机\\player.png", 50, 50, player.x, player.y);
	
	wostringstream outs;
	outs << L"x:"<<player.x<<" y:"<<player.y<<L" DletaTime:"<<theTimer.DeltaTime();
	SetWindowText(theWindow.getHwnd(), outs.str().c_str());
	
	RenderTarget->EndDraw();
}
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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_ACTIVATE:
	{
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			theTimer.Stop();
		}
		else
		{
			theTimer.Start();
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
