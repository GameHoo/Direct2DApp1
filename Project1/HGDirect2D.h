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
#include<wincodec.h>
#pragma comment(lib, "Windowscodecs.lib" )
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
	void DrawBitmapFromFile(WCHAR* FileName, UINT width, UINT height,float x,float y)
	{
		IWICBitmapDecoder *pDecoder = NULL;
		IWICBitmapFrameDecode *pSource = NULL;
		IWICStream *pStream = NULL;
		IWICFormatConverter *pConverter = NULL;
		IWICBitmapScaler *pScaler = NULL;
		ID2D1Bitmap *ppBitmap = NULL;
		IWICImagingFactory *piFactory = NULL;
		CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&piFactory)
			);
		piFactory->CreateDecoderFromFilename(FileName,
			0, GENERIC_READ, WICDecodeMetadataCacheOnLoad,
			&pDecoder);
		pDecoder->GetFrame(0,&pSource);
		UINT originalWidth, originalHeight;
		pSource->GetSize(&originalWidth, &originalHeight);
		double rate = originalWidth / (double)originalHeight;
		if (width > height)
		{
			width = static_cast<UINT>(height*rate);
		}
		else
		{
			height = static_cast<UINT>(width / rate);
		}
		piFactory->CreateBitmapScaler(&pScaler);
		pScaler->Initialize(
			pSource,
			width,
			height,
			WICBitmapInterpolationModeCubic
			);
		piFactory->CreateFormatConverter(&pConverter);
		pConverter->Initialize(
			pScaler,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.0,
			WICBitmapPaletteTypeMedianCut
			);
		m_pRenderTarget->CreateBitmapFromWicBitmap(
			pConverter,
			NULL,
			&ppBitmap
			);
		m_pRenderTarget->DrawBitmap(ppBitmap,
			D2D1::RectF(
				x -25.f,
				y- 25.f,
				x+25.f,
				y+25.f
				)
			,1.0);
		SafeRelease(&pDecoder);
		SafeRelease(&pSource);
		SafeRelease(&pStream);
		SafeRelease(&pConverter);
		SafeRelease(&pScaler);
		SafeRelease(&piFactory);
		SafeRelease(&ppBitmap);
	}
};