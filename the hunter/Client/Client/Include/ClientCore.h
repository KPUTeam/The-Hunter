#pragma once

#include "Client.h"
#include "Network.h"
#include "Dx11Core.h"
#include "Core/Dx11Device.h"

DX11_USING

class CClientCore :
	public CDx11Object
{
	SINGLE_DECLARE(CClientCore)

private:
	char	m_strID[ID_SIZE];
	char	m_strPass[PASS_SIZE];
	char	m_strIP[16];
	short	m_sPort;

public:
	const char*	GetIP() const;
	short GetPort() const;
	const char* GetID() const;
	const char* GetPass() const;

public:
	bool Init(HINSTANCE hInst);
	int Run();
};

