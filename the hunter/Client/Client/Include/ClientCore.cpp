#include "ClientCore.h"
#include "SceneScript\MainSceneSC.h"
#include "Dx11Core.h"
#include "Network\NetManager.h"

// ����
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

	// �α��� ���� ������ �о�´�.
	FILE*	pFile = NULL;
	fopen_s(&pFile, "LoginInfo.lif", "rb");
	if (pFile) // ���Ŀ��� �Ѿ�ö�
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

	else { // ���Ӽ����� �Ѽ� �׽�Ʈ �Ҷ� �̿�
		strcpy_s(m_strIP, "127.0.0.1");
		m_sPort = 9001;
		strcpy_s(m_strID, "�̻�");
		strcpy_s(m_strPass, "123");
	}
	// �α��� ���� ������ �����. ��ó���� �Ѿ���� �����̴�.
	int iResult = remove("LoginInfo.lif");

	if (!DX11_GET_SINGLE(CDx11Core)->Init(hInst, RT_HD,
		L"Dx11", L"Dx11", IDI_ICON1, IDI_ICON1, true))
	{
		DX11_DESTROY_SINGLE(CDx11Core);
		return 0;
	}

	// ���� ���� �����Ѵ�.
	CDx11Scene*	pScene = DX11_GET_SINGLE(CDx11SceneManager)->CreateScene("MainScene");
	
	// �� ��ũ��Ʈ�� �߰��ϰ�,
	// �̴��Ѵ�.
	CMainSceneSC* pMainSceneSC = pScene->AddScript<CMainSceneSC>();

	// mainscene���� ������
	CNetManager::GetInst()->GetInGameNetwork()->SetMainScene(pMainSceneSC);

	// �ʱ�ȭ �Ŀ� ���� ��ü�Ѵ�.
	// ReplaceScene() �� �ϸ�, ü���� ������ Release�� �ϱ� ������ SAFE_RELEASE(pScene) �� �ʿ䰡 ����.
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
