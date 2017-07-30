#pragma once
#include "ZEROEventSelect.h"
#include "../Network.h"
#include "../SceneScript/MainSceneSC.h"

class CIngameNetwork :
	public CZEROEventSelect
{
public:
	CIngameNetwork();
	~CIngameNetwork();

private:
	class CUserSession* m_pSession;
	CMainSceneSC*	m_pMainScene;

public:
	void SetSession(class CUserSession* pSession);
	void SetMainScene(CMainSceneSC* pMainScene);

public:
	virtual VOID	OnIoRead(VOID);
	virtual VOID	OnIoConnected(VOID);
	virtual VOID	OnIoDisconnected(VOID);

private:
	void CreateUser(BYTE* pPacket, DWORD dwLength);
};

