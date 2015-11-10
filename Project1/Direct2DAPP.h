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
	//���ھ��
	HWND m_hwnd;
	//ʹ�� ID2D1Factory �ɴ��� Direct2D ��Դ
	ID2D1Factory* m_pDirect2dFactory;
	//ʹ�ó�����Ŀ����Ƹ�����״
	ID2D1HwndRenderTarget* m_pRenderTarget;
	//��ɫ����
	ID2D1SolidColorBrush* m_pLightSlateGrayBrush;
	//����
	ID2D1SolidColorBrush* m_pCornflowerBlueBrush;
public:
	Direct2DApp();
	//�ͷ���Դ
	~Direct2DApp();

	//��ʼ������
	HRESULT Initialize();
	

	//��Ϣѭ��
	void RunMessageLoop();

private:
	//��ʼ�����豸�޹���Դ
	HRESULT CreateDeviceIndependentResources();

	//��ʼ�����豸�й���Դ
	HRESULT CreateDeviceResources();

	//�ͷ��豸��Դ
	void DiscardDeviceResources();

	//��������
	HRESULT OnRender();

	//���ڴ�С���ı�
	void OnResize(
		UINT width,
		UINT height
		);

	//����ѭ��
	static LRESULT CALLBACK WndProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
		);
};