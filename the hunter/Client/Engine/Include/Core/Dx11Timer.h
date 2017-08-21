#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

class DX11_DLL CDx11Timer :
	public CDx11Object
{
private:
	friend class CDx11TimerManager;

private:
	CDx11Timer();
	~CDx11Timer();

private:
	LARGE_INTEGER	m_tSecond;
	LARGE_INTEGER	m_tCount;
	float			m_fDeltaTime;
	float			m_fFPS;
	float			m_fFPSTime;
	int				m_iFrameCount;
	int				m_iFrameLimitCount;

public:
	float GetDeltaTime();
	float GetFPS();

public:
	bool Init();
	void Update();

private:
	void UpdateTime();
	void UpdateFPS();
};

DX11_END
