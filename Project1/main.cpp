#include<iostream>
#include<stdio.h>
#include"HGWindow.h"
#include "HGDirect2D.h"
#include<Windows.h>
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void Render();
int width = 800;
int height = 600;
HGWindow theWindow;
HGDirect2D theDirect2D;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	
	theWindow.HG_Init_Window(L"´ò·É»ú", WndProc, hInstance, nCmdShow,width,height);

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
	theDirect2D.Init(theWindow.getHwnd());
	ID2D1RenderTarget* RenderTarget =static_cast<ID2D1RenderTarget*>( theDirect2D.Get_RenderTarget());
	IDWriteFactory* WriteFactory;
	ID2D1SolidColorBrush *brush1;
	IDWriteTextFormat * TextFormat;
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&WriteFactory));
	WriteFactory->CreateTextFormat(
		L"Gabriola",                // Font family name.
		NULL,                       // Font collection (NULL sets it to use the system font collection).
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		72.0f,
		L"en-us",
		&TextFormat
		);
	TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
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
	RenderTarget->DrawTextW(
		L"Hello World",
		strlen("Hello World"),
		TextFormat,
		layoutRect,
		brush1
		);
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
		Render();
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
