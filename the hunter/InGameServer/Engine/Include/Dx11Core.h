#pragma once
#include "Dx11Object.h"

DX11_BEGIN

class DX11_DLL CDx11Core :
	public CDx11Object
{
private:
	class CDx11Window*	m_pWindow;

private:
	float		m_fGravity;

public:
	float GetGravity()	const;

public:
	bool Init(HINSTANCE hInst, RESOLUTION_TYPE eType,
		TCHAR* pTitle, TCHAR* pClass, int iIconID, int iSmallIconID,
		bool bWindowMode = true, WNDPROC wndProc = NULL);
	bool Init(HWND hWnd, RESOLUTION_TYPE eType, bool bWindowMode);
	int Run();
	int RunTool();
	void Logic();
	void LogicUpdate(float fDeltaTime);

private:
	void Input(float fTime);
	int Update(float fTime);
	int LateUpdate(float fTime);
	void Collision(float fTime);
	void Render(float fTime);

public:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	static bool	m_bRun;

	DX11_SINGLE_DECLARE(CDx11Core)
};

DX11_END