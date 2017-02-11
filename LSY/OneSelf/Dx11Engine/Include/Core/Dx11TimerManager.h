#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

class DX11_DLL CDx11TimerManager :
	public CDx11Object
{
	DX11_DECLARE_SINGLE(CDx11TimerManager)

private:
	unordered_map<string, class CDx11Timer*>	m_mapTimer;

public:
	bool Init();
	CDx11Timer* CreateTimer(const string& strKey);
	CDx11Timer* FindTimer(const string& strKey);
};

DX11_END
