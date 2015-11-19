#pragma once
#include<Windows.h>
class HGWindow
{
private:
	int m_Width = 0;
	int m_Height = 0;
	HWND m_Hwnd = nullptr;
	LPCWSTR m_ClassName = L"WINDOW";
	//×¢²á´°¿ÚÀà

public:
	HWND getHwnd()
	{
		if(m_Hwnd==nullptr)
		{
			throw(L"m_Hwnd==nullptr");
		}
		return m_Hwnd;
	}

	HGWindow();
	~HGWindow();
	void HGWindow::HG_Init_Window(LPCWSTR title, WNDPROC WndProc, HINSTANCE hInstance, int nCmdShow, int width, int height);

};