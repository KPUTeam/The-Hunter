#include "ClientCore.h"
#include "SceneScript\MainSceneSC.h"
#include "Dx11Core.h"
#include "Network\NetManager.h"

// 엔진
#include "Scene/Dx11SceneManager.h"
#include "Scene/Dx11Scene.h"
#include "Core/Dx11PathManager.h"


SINGLE_DEFINITION(CClientCore);

CClientCore::CClientCore()
{
}

CClientCore::~CClientCore()
{
	DESTROY_SINGLE(CNetManager);
	DX11_DESTROY_SINGLE(CDx11Core);
}

const char * CClientCore::GetIP() const
{
	return m_strIP;
}

short CClientCore::GetPort() const
{
	return m_sPort;
}

const char * CClientCore::GetID() const
{
	return m_strID;
}

const char * CClientCore::GetPass() const
{
	return m_strPass;
}

bool CClientCore::Init(HINSTANCE hInst)
{
#ifdef _DEBUG
	SetCurrentDirectory(L"../../Client/Bin_Debug/");
#else
	SetCurrentDirectory(L"../../Client/Bin");
#endif // _DEBUG

	// 로그인 정보 파일을 읽어온다.
	FILE*	pFile = NULL;
	fopen_s(&pFile, "LoginInfo.lif", "rb");
	if (pFile) // 런쳐에서 넘어올때
	{
		memset(m_strIP, 0, 16);
		memset(m_strID, 0, ID_SIZE);
		memset(m_strPass, 0, PASS_SIZE);

		fread(m_strIP, 16, 1, pFile);
		fread(&m_sPort, 2, 1, pFile);

		fread(m_strID, ID_SIZE, 1, pFile);
		fread(m_strPass, PASS_SIZE, 1, pFile);

		fclose(pFile);
	}

	else { // 게임서버만 켜서 테스트 할때 이용
		strcpy_s(m_strIP, "127.0.0.1");
		m_sPort = 9001;
		strcpy_s(m_strID, "이상영");
		strcpy_s(m_strPass, "123");
	}
	// 로그인 정보 파일을 지운다. 런처에서 넘어오늘 파일이다.
	int iResult = remove("LoginInfo.lif");

	if (!DX11_GET_SINGLE(CDx11Core)->Init(hInst, RT_HD,
		L"Dx11", L"Dx11", IDI_ICON1, IDI_ICON1, true))
	{
		DX11_DESTROY_SINGLE(CDx11Core);
		return 0;
	}

	// 메인 씬을 생성한다.
	CDx11Scene*	pScene = DX11_GET_SINGLE(CDx11SceneManager)->CreateScene("MainScene");
	
	// 씬 스크립트를 추가하고,
	// 이닛한다.
	CMainSceneSC* pMainSceneSC = pScene->AddScript<CMainSceneSC>();

	// mainscene으로 세팅함
	CNetManager::GetInst()->GetInGameNetwork()->SetMainScene(pMainSceneSC);

	// 초기화 후에 씬을 교체한다.
	// ReplaceScene() 을 하면, 체인지 씬에서 Release를 하기 때문에 SAFE_RELEASE(pScene) 할 필요가 없다.
	DX11_GET_SINGLE(CDx11SceneManager)->ReplaceScene(pScene);

	SAFE_RELEASE(pMainSceneSC);
	//SAFE_RELEASE(pScene);

	return true;
}

int CClientCore::Run()
{
	int	iRet = DX11_GET_SINGLE(CDx11Core)->Run();

	return iRet;
}
