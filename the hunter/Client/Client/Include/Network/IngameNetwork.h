#pragma once
#include "ZEROEventSelect.h"
#include "../Network.h"
#include "../SceneScript/MainSceneSC.h"
#include "../SceneScript/InGameSceneSC.h"

class CIngameNetwork :
	public CZEROEventSelect
{
public:
	CIngameNetwork();
	~CIngameNetwork();

private:
	class CUserSession* m_pSession;
	CMainSceneSC*	m_pMainScene;
	CInGameSceneSC*	m_pInGameScene;

public:
	void SetSession(class CUserSession* pSession);
	void SetMainScene(CMainSceneSC* pMainScene);
	void SetInGameScene(CInGameSceneSC* pInGameScene);

public:
	virtual VOID	OnIoRead(VOID);
	virtual VOID	OnIoConnected(VOID);
	virtual VOID	OnIoDisconnected(VOID);

public:
	void CreateMyCharacter(BYTE* pPacket, DWORD dwLength);	// 로그인하면 나를 그림
	void OldCreateNewUser(BYTE* pPacket, DWORD dwLength);	// 기존에 접속해있던 유저에게 새로 접속한 유저의 정보를 보낸 결과를 출력
};

