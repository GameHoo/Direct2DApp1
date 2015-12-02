#pragma once
#include<dinput.h>
#include<Windows.h>
#include<d2d1.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d2d1.lib")
HWND CreateMyWindow(HINSTANCE hInstance, WNDPROC WndProc, int nCmdShow, LPCWSTR title, int width, int height)
{
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
	wc.lpszClassName = L"WINDOWS";
	RegisterClassW(&wc);
	HWND hwnd = CreateWindowW(L"WINDOWS", title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, width, height, nullptr, nullptr, hInstance, nullptr);
	if (!hwnd)
	{
		throw(L"创建窗口失败！");
	}
	else
	{
		//调整窗口客户区大小
		RECT rcWindow;
		RECT rcClient;
		int borderWidth, borderHeight;
		GetWindowRect(hwnd, &rcWindow);
		GetClientRect(hwnd, &rcClient);
		borderWidth = (rcWindow.right - rcWindow.left)
			- (rcClient.right - rcClient.left);
		borderHeight = (rcWindow.bottom - rcWindow.top)
			- (rcClient.bottom - rcClient.top);
		SetWindowPos(hwnd, 0, CW_USEDEFAULT, 0, borderWidth + width,
			borderHeight + height, SWP_NOMOVE | SWP_NOZORDER);
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	return hwnd;
}
ID2D1HwndRenderTarget* CreateRenderTarget(HWND hwnd)
{
	ID2D1Factory* Direct2dFactory = nullptr;
	ID2D1HwndRenderTarget* RenderTarget = nullptr;
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &Direct2dFactory);
	RECT rc;
	GetClientRect(hwnd, &rc);
	D2D1_SIZE_U size = D2D1::SizeU(
		rc.right - rc.left,
		rc.bottom - rc.top
		);
	Direct2dFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hwnd, size),
		&RenderTarget
		);
	Direct2dFactory->Release();
	return RenderTarget;
}
class HGInput
{
	void Create_KeyboardsDevice(HINSTANCE hInstance, HWND hwnd)
	{
		LPDIRECTINPUT8 DirectInput = nullptr;
		DirectInput8Create(hInstance,
			DIRECTINPUT_HEADER_VERSION, IID_IDirectInput8,
			(void**)&DirectInput, NULL);
		DirectInput->CreateDevice(GUID_SysKeyboard, &m_KeyboardsDevice, NULL);
		m_KeyboardsDevice->SetDataFormat(&c_dfDIKeyboard);
		m_KeyboardsDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND);
		DirectInput->Release();
	}
	char m_KeyStateBuffer[256] = { 0 };
	char m_KeyStateBuffer_last[256] = { 0 };
public:
	LPDIRECTINPUTDEVICE8 m_KeyboardsDevice = nullptr;
	void init(HINSTANCE hInstance, HWND hwnd)
	{
		Create_KeyboardsDevice(hInstance, hwnd);
	}
	bool Keyboards_Read()
	{
		memcpy(m_KeyStateBuffer_last, m_KeyStateBuffer, sizeof(m_KeyStateBuffer));
		memset(m_KeyStateBuffer, 0, sizeof(m_KeyStateBuffer));
		HRESULT hr;
		while (true)
		{
			m_KeyboardsDevice->Poll();              // 轮询设备  
			m_KeyboardsDevice->Acquire();          // 获取设备的控制权  
			if (SUCCEEDED(hr = m_KeyboardsDevice->GetDeviceState(sizeof(m_KeyStateBuffer), m_KeyStateBuffer))) break;
			if (hr != DIERR_INPUTLOST || hr != DIERR_NOTACQUIRED) return FALSE;
			if (FAILED(m_KeyboardsDevice->Acquire())) return FALSE;
		}
		return true;
	}
	bool keyDown(int DIK)
	{
		if (m_KeyStateBuffer[DIK] & 0x80)return true;
		return false;
	}
	bool keyPress(int DIK)
	{
		if (m_KeyStateBuffer[DIK] & 0x80 && !m_KeyStateBuffer_last[DIK]&0x80)return true;
		return false;
	}
};
