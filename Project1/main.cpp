#include<iostream>
#include<stdio.h>
#include"HGWindow.h"
#include "HGDirect2D.h"
#include "HGInput.h"
#include"GameTimer.hpp"
#include<Windows.h>
#include <sstream>
using namespace std;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void CalculateFPS();
void Render();
void Update();
int width = 800;
int height = 600;
HGWindow theWindow;
HGDirect2D theDirect2D;
HGInput theInput;
GameTimer theTimer;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{

	theWindow.HG_Init_Window(L"雷霆战机", WndProc, hInstance, nCmdShow, width, height);
	theDirect2D.Init(theWindow.getHwnd());
	theInput.init(hInstance, theWindow.getHwnd());
	theTimer.Reset();
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

		Render();
		theTimer.Tick();
	}

	return 0;
}
//逻辑计算
void Update()
{
	
}
//渲染
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
	if (theInput.isKeyDown(DIK_UPARROW))
	{
		RenderTarget->DrawTextW(
			L"按下了",
			strlen("按下了"),
			theDirect2D.Get_WriteTextFormat(),
			layoutRect,
			brush1
			);
		
	}
	else
	{
		RenderTarget->DrawTextW(
			L"弹起了",
			strlen("弹起了"),
			theDirect2D.Get_WriteTextFormat(),
			layoutRect,
			brush1
			);
		
	}

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
