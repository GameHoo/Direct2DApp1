/*
创建D2D需要的各种资源
ID2D1Factory RenderTarget
*/
#pragma once
#include<Windows.h>
#include<d2d1.h>
#include<D2d1helper.h>
#pragma comment(lib,"d2d1.lib")
#include<dwrite.h>
#pragma comment(lib,"dwrite.lib")
class HGDirect2D
{
private:
	//窗口句柄
	HWND m_hwnd=nullptr;
	//使用 ID2D1Factory 可创建 Direct2D 资源
	ID2D1Factory* m_pDirect2dFactory=nullptr;
	//IDWriteFactory 创建 DirectWrite 资源
	IDWriteFactory* m_WriteFactory = nullptr;
	//TextFormat
	IDWriteTextFormat * m_TextFormat = nullptr;
	//使用呈现器目标绘制各种形状
	ID2D1HwndRenderTarget* m_pRenderTarget = nullptr;
	
	//初始化与设备无关资源
	void CreateDeviceIndependentResources();

	//初始化与设备有关资源
	void CreateDeviceResources();


public:
	HGDirect2D()
	{}
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
	ID2D1Factory* Get_Factory()
	{
		if(m_pDirect2dFactory==nullptr)
		{
			throw(L"m_pDirect2dFactory==nullptr");
		}
		return m_pDirect2dFactory;
	}
	ID2D1RenderTarget* Get_RenderTarget()
	{
		if (m_pRenderTarget == nullptr)
		{
			throw(L"m_pRenderTarget == nullptr");
		}
		return m_pRenderTarget;
	}
	IDWriteFactory* Get_WriteFactory()
	{
		if (m_WriteFactory == nullptr)
		{
		throw(L"m_WriteFactory==nullptr");
		}
		return m_WriteFactory;
	}
	IDWriteTextFormat* Get_WriteTextFormat()
	{
		if (m_TextFormat == nullptr)
		{
			throw(L"m_TextFormat==nullptr");
		}
		return m_TextFormat;
	}
};