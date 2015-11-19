#include<iostream>
#include<stdio.h>
#include"HGWindow.h"
#include "HGDirect2D.h"
#include "HGInput.h"
#include<Windows.h>
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void Render();
int width = 800;
int height = 600;
HGWindow theWindow;
HGDirect2D theDirect2D;
HGInput theInput;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	
	theWindow.HG_Init_Window(L"打飞机", WndProc, hInstance, nCmdShow,width,height);
	theDirect2D.Init(theWindow.getHwnd());
	theInput.init(hInstance, theWindow.getHwnd());
	MSG msg;
	while (true)
	{
		if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if(msg.message==WM_QUIT)
			{
				break;
			}
			DispatchMessage(&msg);
		}
		//TranslateMessage(&msg);
		Render();
	}
	
	return 0;
}
void Render()
{

	ID2D1RenderTarget* RenderTarget =static_cast<ID2D1RenderTarget*>( theDirect2D.Get_RenderTarget());
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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_PAINT:
	{
		/*PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);*/
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
