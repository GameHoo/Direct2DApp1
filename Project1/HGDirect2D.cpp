#include"HGDirect2D.h"
void HGDirect2D::CreateDeviceIndependentResources()
{
	//创建工厂
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&m_WriteFactory));
	//WIC
	CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&m_WICFactory)
		);
	
}
void HGDirect2D::CreateDeviceResources()
{
	HRESULT hr = S_OK;
	if (!m_pRenderTarget)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
			);

		// Create a Direct2D render target.
		hr = m_pDirect2dFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwnd, size),
			&m_pRenderTarget
			);
		//CreateTextFormat
		m_WriteFactory->CreateTextFormat(
			L"Gabriola",                // Font family name.
			NULL,                       // Font collection (NULL sets it to use the system font collection).
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			72.0f,
			L"en-us",
			&m_TextFormat
			);
		m_TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

		/*
		if (SUCCEEDED(hr))
		{
			// Create a gray brush.
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::LightSlateGray),
				&m_pLightSlateGrayBrush
				);
		}
		if (SUCCEEDED(hr))
		{
			// Create a blue brush.
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
				&m_pCornflowerBlueBrush
				);
		}*/
	}
}



void HGDirect2D::DrawSprit(spirit* the)
{
	ID2D1Bitmap* pbitmap;
	int width = the->width;
	int height = the->height;
	float x = the->x;
	float y = the->y;
	float angle = the->getAngle();
	m_pRenderTarget->CreateBitmapFromWicBitmap(spirit::Resource[the->id],nullptr,&pbitmap);
	
	m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(
		angle,
		D2D1::Point2F(x, y)
		));
	m_pRenderTarget->DrawBitmap(
		pbitmap,
		D2D1::RectF(
			x - width / 2,
			y - height / 2,
			x + width / 2,
			y + height / 2
			)

		);
	m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	pbitmap->Release();
}
