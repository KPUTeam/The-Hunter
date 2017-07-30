#include "IngameNetwork.h"
#include "NetManager.h"
#include "UserSession.h"
#include "ZEROStream.h"
#include "Dx11Value.h"
#include "../SceneScript/MainSceneSC.h"
#include "Scene/Dx11SceneManager.h"
#include "Scene/Dx11Scene.h"
#include "Scene/Dx11Layer.h"
#include "GameObject/Dx11GameObject.h"
#include "Component/Dx11Transform.h"
#include "Component/Dx11Renderer.h"
#include "Component/Dx11Material.h"
#include "../ObjScript/Player/User.h"
#include "Component/Dx11ColliderSphere.h"
#include "Resources/Dx11ResourcesManager.h"

CIngameNetwork::CIngameNetwork() :
	m_pSession(NULL)
{
}


CIngameNetwork::~CIngameNetwork()
{
	// SAFE_RELEASE(m_pSessio);
	SAFE_RELEASE(m_pMainScene);
}

void CIngameNetwork::SetSession(CUserSession * pSession)
{
	m_pSession = pSession;
}

void CIngameNetwork::SetMainScene(CMainSceneSC * pMainScene)
{
	m_pMainScene = pMainScene;
}

VOID CIngameNetwork::OnIoRead(VOID)
{
	if (m_pSession->ReadPacketForEventSelect())
	{
		DWORD	dwHeader = 0;
		BYTE	Packet[PACKET_SIZE] = {};
		DWORD	dwLength;

		CDx11Scene* pScene = DX11_GET_SINGLE(CDx11SceneManager)->GetCurrentScene();
		//		CMainSceneSC*	pMainScene = (CMainSceneSC*)pScene->FindMainScene("MainScene");

		while (m_pSession->GetPacket(dwHeader, Packet, dwLength))
		{
			switch (dwHeader)
			{
			case PH_LOGIN_FAILED:
			{
				m_pSession->End();
			}
			break;

			case PH_LOGIN_SUCCEEDED:
			{
				INT64	iSerialNumber;
				memcpy(&iSerialNumber, Packet, 8);
				m_pSession->SetSerialNumber(iSerialNumber);
				int a = 10;
			}
			break;

			case PH_DISCONNECT:
			{

			}
			break;

			case PH_USER_LOGIN:
				CreateUser(Packet, dwLength);
				break;

			case PH_MY_MOVE:
				break;

			case PH_OTHER_MOVE:
				break;
			}
		}
	}
}

VOID CIngameNetwork::OnIoConnected(VOID)
{
	return VOID();
}

VOID CIngameNetwork::OnIoDisconnected(VOID)
{
	return VOID();
}

void CIngameNetwork::CreateUser(BYTE * pPacket, DWORD dwLength)
{
	CZEROStreamSP	stream;
	stream->SetBuffer(pPacket);
	INT64	iSerialNumber;

	stream->ReadInt64(&iSerialNumber);

	Vec3	vPos, vScale, vRot;

	stream->ReadFloat(&vScale.x);
	stream->ReadFloat(&vScale.y);
	stream->ReadFloat(&vScale.z);

	stream->ReadFloat(&vRot.x);
	stream->ReadFloat(&vRot.y);
	stream->ReadFloat(&vRot.z);

	stream->ReadFloat(&vPos.x);
	stream->ReadFloat(&vPos.y);
	stream->ReadFloat(&vPos.z);

	// 유저 객체를 생성해준다.
	CDx11GameObject*	pUser = CDx11GameObject::Create("UserObject");
	CDx11Transform*	pUserTr = pUser->GetTransform();

	//	pUserTr->SetWorldScale(vScale);
	pUserTr->SetWorldRot(vRot);
	pUserTr->SetWorldPos(rand() % 3 - 1.5f, rand() % 3 - 1.5f, rand() % 3 - 1.5f);

	SAFE_RELEASE(pUserTr);

	// 렌더러를 생성한다.
	CDx11Renderer*	pRenderer = pUser->AddComponent<CDx11Renderer>("UserRenderer");
	pRenderer->SetMesh("PlayerMesh");
	pRenderer->SetShader("TexShader");
	SAFE_RELEASE(pRenderer);

	/*CDx11Renderer* pRenderer = pUser->AddComponent<CDx11Renderer>("UserRenderer");

	vector<vector<TCHAR*>>	vecTexName;
	vector<TCHAR*>	vecTex;
	vecTexName.push_back(vecTex);

	vecTexName[0].push_back(L"Assault_droid_brass_D.tga");
	CDx11Mesh* pMesh = DX11_GET_SINGLE(CDx11ResourcesManager)->LoadMesh("PlayerMesh",
	L"Assault_droid.FBX", vecTexName);

	pRenderer->SetMesh("PlayerMesh");
	pRenderer->SetShader(DEFAULT_BUMP_SHADER);
	SAFE_RELEASE(pRenderer);

	CDx11ColliderSphere* pSphere = pUser->AddComponent <CDx11ColliderSphere>("UserColl");
	pSphere->SetInfo(Vec3Zero, 0.8f);
	SAFE_RELEASE(pSphere);*/

	CUser* pUserScript = pUser->AddComponent<CUser>("User");
	pUserScript->SetSerialNumber(iSerialNumber);
	SAFE_RELEASE(pUserScript);
	GET_SINGLE(CNetManager)->AddLoginObj(pUser);
}
