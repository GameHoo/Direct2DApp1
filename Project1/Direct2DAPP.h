#pragma once
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files:
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>

#include <d2d1.h>
#pragma comment(lib,"d2d1.lib")
#include <d2d1helper.h>
#include <dwrite.h>
#pragma comment(lib,"dwrite.lib")
#include <wincodec.h>
#pragma comment(lib,"windowscodecs.lib")
using namespace std;
template<class Interface>
inline void SafeRelease(Interface **ppInterfaceToRelease)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = NULL;
	}
}
#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}} while(0)
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif
#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

class Direct2DApp
{
private:
	//窗口句柄
	HWND m_hwnd;
	//使用 ID2D1Factory 可创建 Direct2D 资源
	ID2D1Factory* m_pDirect2dFactory;
	//使用呈现器目标绘制各种形状
	ID2D1HwndRenderTarget* m_pRenderTarget;
	//灰色画笔
	ID2D1SolidColorBrush* m_pLightSlateGrayBrush;
	//画笔
	ID2D1SolidColorBrush* m_pCornflowerBlueBrush;
public:
	Direct2DApp();
	//释放资源
	~Direct2DApp();

	//初始化窗口
	HRESULT Initialize();
	

	//消息循环
	void RunMessageLoop();

private:
	//初始化与设备无关资源
	HRESULT CreateDeviceIndependentResources();

	//初始化与设备有关资源
	HRESULT CreateDeviceResources();

	//释放设备资源
	void DiscardDeviceResources();

	//绘制内容
	HRESULT OnRender();

	//窗口大小被改变
	void OnResize(
		UINT width,
		UINT height
		);

	//窗口循环
	static LRESULT CALLBACK WndProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
		);
};