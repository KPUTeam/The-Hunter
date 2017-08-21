#include "IngameNetwork.h"
#include "NetManager.h"
#include "UserSession.h"
#include "ZEROStream.h"
#include "Dx11Value.h"
#include "../SceneScript/MainSceneSC.h"
#include "../Scenescript/InGameSceneSC.h"
#include "Scene/Dx11SceneManager.h"
#include "Scene/Dx11Scene.h"
#include "Scene/Dx11Layer.h"
#include "GameObject/Dx11GameObject.h"
#include "Component/Dx11Transform.h"
#include "Component/Dx11Renderer.h"
#include "Component/Dx11Material.h"
#include "../ObjScript/Player/User.h"
#include "../ObjScript/Tank/TankInitSC.h"
#include "Component/Dx11ColliderSphere.h"
#include "Resources/Dx11ResourcesManager.h"

DX11_USING

bool startFlag = false;

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

void CIngameNetwork::SetInGameScene(CInGameSceneSC * pInGameScene)
{
	m_pInGameScene = pInGameScene;
}

VOID CIngameNetwork::OnIoRead(VOID)
{
	if (m_pSession->ReadPacketForEventSelect())
	{
		DWORD	dwHeader = 0;
		BYTE	Packet[PACKET_SIZE] = {};
		DWORD	dwLength;

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
				m_pSession->SetConnected(TRUE);
				int a = 10;
			}
			break;

			case PH_MY_INIT:
				m_pInGameScene->CreateTank(Packet);
				break;

			case PH_MONSTER_POS:
				m_pInGameScene->CreateMonster(Packet);
				break;

			case PH_DISCONNECT:
			{

			}
			break;

			case PH_USER_LOGIN:
				OldCreateNewUser(Packet, dwLength);
				break;

			case PH_START_GAME:
				startFlag = true;
				break;

			case PH_MY_MOVE_SUCCEEDED: // 다른 사람에게 나의 움직임을 보냄
				m_pInGameScene->MoveOtherUser(Packet);
				break;

			case PH_MY_ROTATE_SUCCEEDED:
				m_pInGameScene->RotateOtherUser(Packet);
				break;

			case PH_MY_HEAD_ROTATE_SUCCEEDED:
				m_pInGameScene->RotateHeadOtherUser(Packet);
				break;

			case PH_MY_SHOOT_SUCCEEDED:
				m_pInGameScene->ShootOtherUser(Packet);
				break;

			case PH_USER_WIN_RET:
				m_pInGameScene->ChangeScene();
				break;

			case PH_USER_LOSE_RET:
				m_pInGameScene->ChangeScene();
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

void CIngameNetwork::CreateMyCharacter(BYTE * pPacket, DWORD dwLength)
{

}

void CIngameNetwork::OldCreateNewUser(BYTE * pPacket, DWORD dwLength)
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

	//pUserTr->SetWorldScale(vScale);
	pUserTr->SetWorldRot(vRot);
	pUserTr->SetWorldPos(vPos);
	SAFE_RELEASE(pUserTr);


	CUser* pUserScript = pUser->AddComponent<CUser>("User");
	pUserScript->SetSerialNumber(iSerialNumber);
	pUserScript->AddUserObj(pUserScript);
	SAFE_RELEASE(pUserScript);
	GET_SINGLE(CNetManager)->AddLoginObj(pUser);

}
