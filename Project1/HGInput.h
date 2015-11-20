#pragma once
#define INITGUID
#include<dinput.h>
#include<Windows.h>

#pragma comment(lib,"dinput8.lib")

class HGInput
{
private:
	LPDIRECTINPUT8 m_DirectInput = nullptr;
	LPDIRECTINPUTDEVICE8 m_MouseDevice = nullptr;
	LPDIRECTINPUTDEVICE8 m_KeyboardsDevice = nullptr;
	char m_KeyStateBuffer[256] = { 0 };
	template<class Interface>
	inline void SafeRelease(Interface **ppInterfaceToRelease)
	{
		if (*ppInterfaceToRelease != NULL)
		{
			(*ppInterfaceToRelease)->Release();

			(*ppInterfaceToRelease) = NULL;
		}
	}
	BOOL Device_Read(IDirectInputDevice8*pDIDevice, void* pBuffer, long lSize);
public:
	HGInput()
	{
		
	}
	~HGInput()
	{
		
	}
	void init(HINSTANCE hInstance, HWND hwnd);
	
	BOOL isKeyDown(int DIK);

	
};