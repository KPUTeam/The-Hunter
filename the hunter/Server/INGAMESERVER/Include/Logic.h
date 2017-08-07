#pragma once

#include "Network.h"
#include "GameObject/Dx11GameObject.h"
#include "Component/Dx11Transform.h"

DX11_USING

class CLogic
{
public:
	CLogic();
	~CLogic();

private:
	HANDLE	m_hThread;
	bool	m_bLoop;
	float	m_fGlobalTime;
	float	m_fStartTime;

	bool	m_bMonster = false;

public:
	bool Init();
	void Run();

public:
	static UINT __stdcall LogicThreadFunc(void* pArg);

};

