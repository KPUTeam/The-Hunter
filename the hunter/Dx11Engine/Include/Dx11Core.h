#pragma once
#include "Dx11Object.h"

DX11_BEGIN

class DX11_DLL CDx11Core :
	public CDx11Object
{
private:
	CDx11Core();
	~CDx11Core();

private:
	static CDx11Core*	m_pInst;

public: 
	static CDx11Core* GetInst();
	static void DestroyInst();

private:
	class CDx11Window*	m_pWindow;		// 전방선언
	static bool			m_bLoop;

public:
	bool Init(TCHAR* pTitle, TCHAR* pClass, HINSTANCE hInst,
		int iIconID, int iSmallIconID, RESOLUTION_TYPE eRT,
		bool bWindowMode, WNDPROC pProc = NULL);
	int Run();

private:
	void Logic();
	void Input(float fTime);
	int Update(float fTime);
	int LateUpdate(float fTime);
	void Collision(float fTime);
	void Render(float fTime);

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

DX11_END