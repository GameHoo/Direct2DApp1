#include "HGWindow.h"
HGWindow::HGWindow()
{
	
}
HGWindow::~HGWindow()
{
	
}

void HGWindow::HG_Init_Window(LPCWSTR title, WNDPROC WndProc, HINSTANCE hInstance, int nCmdShow,int width,int height)
{
	m_Width = width;
	m_Height = height;
	WNDCLASSW wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = m_ClassName;
	RegisterClassW(&wc);
	m_Hwnd = CreateWindowW(m_ClassName, title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, m_Width, m_Height, nullptr, nullptr, hInstance, nullptr);
	if (!m_Hwnd)
	{
		throw(L"创建窗口失败！");
	}
	else
	{
		//调整窗口客户区大小
		RECT rcWindow;
		RECT rcClient;
		int borderWidth, borderHeight;
		GetWindowRect(m_Hwnd, &rcWindow);
		GetClientRect(m_Hwnd, &rcClient);
		borderWidth = (rcWindow.right - rcWindow.left)
			- (rcClient.right - rcClient.left);
		borderHeight = (rcWindow.bottom - rcWindow.top)
			- (rcClient.bottom - rcClient.top);
		SetWindowPos(m_Hwnd, 0, CW_USEDEFAULT, 0, borderWidth + m_Width,
			borderHeight + m_Height, SWP_NOMOVE | SWP_NOZORDER);
	}
	ShowWindow(m_Hwnd, nCmdShow);
	UpdateWindow(m_Hwnd);
}
