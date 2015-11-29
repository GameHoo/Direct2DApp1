#include<iostream>
#include<stdio.h>
#include"HGWindow.h"
#include "HGDirect2D.h"
#include "HGInput.h"
#include"GameTimer.hpp"
#include "spirits.hpp"
#include<Windows.h>
#include <sstream>
#include <vector>
#include<random>
using namespace std;


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
//�Ƿ�ʱ�����ɵл�1
bool isTimeToEnemy1(float deltatime);
void CalculateFPS();
void Render();
void Update(float Delta);
int width = 800;
int height = 600;
HGWindow theWindow;
HGDirect2D theDirect2D;
HGInput theInput;
GameTimer theTimer;
//�����б�
vector<spirit*> Spirit_List;
bool isRun = true;
random_device rd;
mt19937 mt(rd());
map<wstring, IWICFormatConverter*> temp;
map<wstring, IWICFormatConverter*> &spirit::Resource = temp;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	
	theWindow.HG_Init_Window(L"����ս��", WndProc, hInstance, nCmdShow, width, height);
	theDirect2D.Init(theWindow.getHwnd());
	theInput.init(hInstance, theWindow.getHwnd());
	theTimer.Reset();
	//�������
	Spirit_List.push_back(new player());

	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			DispatchMessage(&msg);
		}
		if (isRun)
		{
			//ִ���߼�
			Update(theTimer.DeltaTime());
			Render();
			theTimer.Tick();
		}
	}
	//_CrtDumpMemoryLeaks();
	return 0;
}
//�߼�����
void Update(float Delta)
{
	//��ȡ�û�����
	int keydown = 0;
	if(theInput.isKeyDown(DIK_UPARROW))
	{
		keydown += 1;
	}
	if(theInput.isKeyDown(DIK_DOWNARROW))
	{
		keydown += 2;	
	}
	if (theInput.isKeyDown(DIK_LEFTARROW))
	{
		keydown += 4;	
	}
	if (theInput.isKeyDown(DIK_RIGHTARROW))
	{
		keydown += 8;
	}
	//���� 
	//���ӵ�Ҫ��Ĳ���
	wstring bullet_ID;
	int bullet_Number;
	int bullet_Source_x;
	int bullet_Source_y;
	bool bullet_IsEnemy;
	int angle_add = 20;//����ӵ�����Ƕ�����
	for (vector<spirit*>::iterator i=Spirit_List.begin(); i !=Spirit_List.end();i=i+1)
	{
		spirit* the = *i;
		the->action(theTimer.DeltaTime(), keydown);
		
		if(the->shoot(bullet_ID,bullet_Number, bullet_IsEnemy,theTimer.DeltaTime()))
		{
			//�����ӵ�
			if(bullet_Number&1) //���� �м䷢���ӵ�
			{
				spirit* a_bullet = bullet::CreatBullet(bullet_ID, vector2D::AngleToVector(the->getAngle()), the->x, the->y, bullet_IsEnemy);
				i=Spirit_List.insert(i,a_bullet);
				i++;
				bullet_Number--;
			}
			int left_or_Right = 0;//0 left 1 right
			int add = angle_add;
			while (bullet_Number)
			{
				float bullet_angle;//right
				{
					bullet_angle = the->speed.VecoterToAngle() + add;
					bullet_angle = (int)(bullet_angle + 360) % 360;
					i = Spirit_List.insert(i, bullet::CreatBullet(bullet_ID, vector2D::AngleToVector(bullet_angle), the->x, the->y, bullet_IsEnemy));
					i++; 
				}
				{//left
					bullet_angle = the->speed.VecoterToAngle() -  add;
					bullet_angle = (int)(bullet_angle + 360) % 360;
					i = Spirit_List.insert(i, bullet::CreatBullet(bullet_ID, vector2D::AngleToVector(bullet_angle), the->x, the->y, bullet_IsEnemy));
					i++;
				}
				
				bullet_Number = bullet_Number - 2;
				add +=angle_add;
			}
		}
	}

	
	
	
	
	//ɾ��HP=0�ľ���
	for (vector<spirit*>::iterator i = Spirit_List.begin(); i != Spirit_List.end();)
	{
		spirit* temp = *i;
		if(temp->IsDead())
		{
			if(temp->id==L"player")//��Ϸ����
			{
				i++;
			}
			else
			{
				delete temp;
				i = Spirit_List.erase(i);
			}
			
		}
		else
		{
			i++;
		}
	}
}



void Render()
{
	
	ID2D1RenderTarget* RenderTarget = static_cast<ID2D1RenderTarget*>(theDirect2D.m_pRenderTarget);
	ID2D1SolidColorBrush *brush1;

	RenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF(0xececee)),
		&brush1
		);

	RenderTarget->BeginDraw();
	RenderTarget->Clear(D2D1::ColorF(D2D1::ColorF(0xacd0ce)));
	RenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	//���Ʋ˵�
	RenderTarget->DrawLine(D2D1::Point2F(700.f, 0.f),
		D2D1::Point2F(700.f, 600.f), brush1, 200.f);
	
	//����ÿ������
	for (vector<spirit*>::iterator i = Spirit_List.begin(); i != Spirit_List.end(); i++)
	{
		spirit* temp = *i;
		//RenderTarget->BeginDraw();
		theDirect2D.DrawSprit(temp);
		//RenderTarget->EndDraw();
	}
	RenderTarget->EndDraw();
}
//����FPS
void CalculateFPS()
{
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;
	frameCnt++;
	if ((theTimer.TotalTime() - timeElapsed) >= 1.f)//1.0msˢ��һ��
	{
		float fps = (float)frameCnt;
		float mspf = 1000.0f / fps;

		wostringstream outs;
		outs.precision(6);
		outs << L"����ս��" << L"   "
			<< L"FPS: " << fps << L"   "
			<< L"Frame Time:" << mspf << L"(ms)";
		SetWindowText(theWindow.getHwnd(), outs.str().c_str());
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}
//����ѭ��
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_ACTIVATE:
	{
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			theTimer.Stop();
			isRun = false;
		}
		else
		{
			theTimer.Start();
			isRun = true;
		}
	}
	break;
	case WM_PAINT:
	{
 	   ValidateRect(theWindow.getHwnd(), nullptr);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_MOVE:
		//Render();
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);

	}
	return 0;
}
