/*
����D2D��Ҫ�ĸ�����Դ
ID2D1Factory RenderTarget
*/
#pragma once
#include<Windows.h>
#include<d2d1.h>
#include<D2d1helper.h>
#pragma comment(lib,"d2d1.lib")
#include<dwrite.h>
#pragma comment(lib,"dwrite.lib")
#include<wincodec.h>
#pragma comment(lib, "Windowscodecs.lib" )
#include<vector>
#include "spirits.hpp"
#include <sstream>
#include <string>

using namespace std;
class HGDirect2D
{
public:
	//���ھ��
	HWND m_hwnd=nullptr;
	//ʹ�� ID2D1Factory �ɴ��� Direct2D ��Դ
	ID2D1Factory* m_pDirect2dFactory=nullptr;
	//IDWriteFactory ���� DirectWrite ��Դ
	IDWriteFactory* m_WriteFactory = nullptr;
	//TextFormat
	IDWriteTextFormat * m_TextFormat = nullptr;
	//ʹ�ó�����Ŀ����Ƹ�����״
	ID2D1HwndRenderTarget* m_pRenderTarget = nullptr;
	//��ʼ�����豸�޹���Դ
	IWICImagingFactory * m_WICFactory = nullptr;
	//ͼƬ��Դ��
	vector<ID2D1Bitmap*> m_bmps;
	void CreateDeviceIndependentResources();

	//��ʼ�����豸�й���Դ
	void CreateDeviceResources();



	HGDirect2D()
	{
		
	}
	~HGDirect2D()
	{
		SafeRelease(&m_pRenderTarget);
		SafeRelease(&m_pDirect2dFactory);
		SafeRelease(&m_WriteFactory);
		SafeRelease(&m_TextFormat);
	}
	template<class Interface>
	inline void SafeRelease(Interface **ppInterfaceToRelease)
	{
		if (*ppInterfaceToRelease != NULL)
		{
			(*ppInterfaceToRelease)->Release();

			(*ppInterfaceToRelease) = NULL;
		}
	}
	void Init(HWND hwnd)
	{
		m_hwnd = hwnd;
		CreateDeviceIndependentResources();
		CreateDeviceResources();
		
	}
	

	void DrawSprit(spirit* spirit);
	
};