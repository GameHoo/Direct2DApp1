#include"HGInput.h"

void HGInput::init(HINSTANCE hInstance, HWND hwnd)
{
	DirectInput8Create(hInstance,
		DIRECTINPUT_HEADER_VERSION, IID_IDirectInput8,
		(void**)&m_DirectInput, NULL);
	m_DirectInput->CreateDevice(GUID_SysMouse, &m_MouseDevice, NULL);
	m_MouseDevice->SetDataFormat(&c_dfDIMouse);
	m_MouseDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND);

	m_DirectInput->CreateDevice(GUID_SysKeyboard, &m_KeyboardsDevice, NULL);
	m_KeyboardsDevice->SetDataFormat(&c_dfDIKeyboard);
	m_KeyboardsDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND);
}
BOOL HGInput::Device_Read(IDirectInputDevice8*pDIDevice, void* pBuffer, long lSize)
{
	HRESULT hr;
	while (true)
	{
		pDIDevice->Poll();              // 轮询设备  
		pDIDevice->Acquire();          // 获取设备的控制权  
		if (SUCCEEDED(hr = pDIDevice->GetDeviceState(lSize, pBuffer))) break;
		if (hr != DIERR_INPUTLOST || hr != DIERR_NOTACQUIRED) return FALSE;
		if (FAILED(pDIDevice->Acquire())) return FALSE;
	}
	return true;
}
BOOL HGInput::isKeyDown(int DIK)
{
	memset(m_KeyStateBuffer, 0, sizeof(m_KeyStateBuffer));
	Device_Read(m_KeyboardsDevice, m_KeyStateBuffer, sizeof(m_KeyStateBuffer));
	if (m_KeyStateBuffer[DIK] & 0x80)return true;
	return false;
}