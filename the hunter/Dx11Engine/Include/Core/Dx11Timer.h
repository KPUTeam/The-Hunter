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
	LARGE_INTEGER	m_tTime;

private:
	double			m_dFrameTime;		//1회 작업이 완료되는 시간

private:
	float			m_fFPS;				//1초 동안 완료되는 총 작업의 수
	float			m_fFPSTime;			
	int				m_iFPSCount;
	int				m_iFPSLimitCount;

public:
	float GetFrameTime();
	float GetFPS();

public:
	bool Init();
	void Update();

private:
	void UpdateTime();
	void UpdateFPS();

};

DX11_END
