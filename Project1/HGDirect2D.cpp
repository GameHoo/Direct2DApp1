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
void HGDirect2D::Init_LoadBMP()
{
	m_bmps.push_back(LoadBMP(L"file/ep_2.png"));
	m_bmps.push_back(LoadBMP(L"file/myb_1.png"));
	m_bmps.push_back(LoadBMP(L"file/enemy1.png"));
}
ID2D1Bitmap* HGDirect2D::LoadBMP(wchar_t* FileName)
{
	IWICBitmapDecoder *pDecoder = NULL;
	IWICBitmapFrameDecode *pSource = NULL;
	IWICStream *pStream = NULL;
	IWICFormatConverter *pConverter = NULL;
	IWICBitmapScaler *pScaler = NULL;
	ID2D1Bitmap *pBitmap = NULL;


	m_WICFactory->CreateDecoderFromFilename(FileName,
		0, GENERIC_READ, WICDecodeMetadataCacheOnLoad,
		&pDecoder);
	pDecoder->GetFrame(0, &pSource);
	UINT originalWidth, originalHeight;
	pSource->GetSize(&originalWidth, &originalHeight);
	//double rate = originalWidth / (double)originalHeight;
	/*
	if (width > height)
	{
	width = static_cast<UINT>(height*rate);
	}
	else
	{
	height = static_cast<UINT>(width / rate);
	}
	*/
	m_WICFactory->CreateBitmapScaler(&pScaler);
	pScaler->Initialize(
		pSource,
		originalWidth,
		originalHeight,
		WICBitmapInterpolationModeCubic
		);

	m_WICFactory->CreateFormatConverter(&pConverter);
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
		&pBitmap
		);

	SafeRelease(&pDecoder);
	SafeRelease(&pSource);
	SafeRelease(&pStream);
	SafeRelease(&pConverter);
	SafeRelease(&pScaler);
	//SafeRelease(&pBitmap);
	return pBitmap;
}
ID2D1Bitmap* HGDirect2D::GetBitmapFromSpirit(Spirit * spirit)
{
	return m_bmps[spirit->id];
}
void HGDirect2D::DrawSprit(Spirit* spirit)
{
	ID2D1Bitmap* pbitmap;
	int width = spirit->size.width;
	int height = spirit->size.height;
	float x = spirit->x;
	float y = spirit->y;
	pbitmap = GetBitmapFromSpirit(spirit);
	m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(
		spirit->getAngle(),
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
}
