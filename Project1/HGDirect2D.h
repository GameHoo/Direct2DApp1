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
	//���ھ��
	HWND m_hwnd=nullptr;
	//ʹ�� ID2D1Factory �ɴ��� Direct2D ��Դ
	ID2D1Factory* m_pDirect2dFactory=nullptr;
	//ʹ�ó�����Ŀ����Ƹ�����״
	ID2D1HwndRenderTarget* m_pRenderTarget = nullptr;
	
	//��ʼ�����豸�޹���Դ
	void CreateDeviceIndependentResources();

	//��ʼ�����豸�й���Դ
	void CreateDeviceResources(HWND hwnd);

	//�ͷ��豸��Դ
	void DiscardDeviceResources();
public:
	HGDirect2D()
	{}
	~HGDirect2D()
	{
		DiscardDeviceResources();
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
		CreateDeviceResources(hwnd);
	}
	void* Get_Factory()
	{
		if(m_pDirect2dFactory==nullptr)
		{
			throw(L"m_pDirect2dFactory==nullptr");
		}
		return m_pDirect2dFactory;
	}
	void* Get_RenderTarget()
	{
		if (m_pRenderTarget == nullptr)
		{
			throw(L"m_pRenderTarget == nullptr");
		}
		return m_pRenderTarget;
	}
};