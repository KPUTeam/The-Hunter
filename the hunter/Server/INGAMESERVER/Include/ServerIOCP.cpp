#include "ServerIOCP.h"
#include "UserSession.h"
#include "NetworkManager.h"
#include "UserManager.h"
#include "Network\ZEROStream.h"
#include "UserScript.h"
#include "MonsterManager.h"
#include "GameObject\Dx11GameObject.h"
#include "Component\Dx11Transform.h"
#include "Component\Dx11ColliderSphere.h"
#include "Component\Dx11Terrain.h"

#include <random>

DX11_USING


CServerIOCP::CServerIOCP()
{
}


CServerIOCP::~CServerIOCP()
{
}

VOID CServerIOCP::OnIoRead(VOID * object, DWORD dataLength)
{
	CUserSession* pSession = (CUserSession*)object;
	DWORD	dwHeader = 0;
	BYTE	PacketArr[PACKET_SIZE] = {};
	DWORD	dwLength = 0;

	if (pSession->ReadPacketForIocp(dataLength))
	{
		while (pSession->GetPacket(dwHeader, PacketArr, dwLength))
		{
			switch (dwHeader)
			{
			case PH_LOGIN:
				UserLogin(pSession, PacketArr);
				UserPos(pSession, PacketArr);
		//		MonsterPos(pSession, PacketArr);
				break;

			case PH_MY_MOVE:
				UserMove(pSession, PacketArr);
				break;

			case PH_MY_ROTATE:
				UserRotate(pSession, PacketArr);
				break;

			case PH_MY_HEAD_ROTATE:
				UserHeadRotate(pSession, PacketArr);
				break;

			case PH_MY_SHOOT:
				UserShoot(pSession, PacketArr);
				break;
			}
		}
	}

	if (!pSession->InitializeReadForIocp())
		pSession->Restart(pSession->GetSocket());
}

VOID CServerIOCP::OnIoWrote(VOID * object, DWORD dataLength)
{
	return VOID();
}

VOID CServerIOCP::OnIoConnected(VOID * object)
{
	CUserSession* pSession = (CUserSession*)object;

	// 접속한 개체의 IO를 IOCP를 통해서 받기 위해서 IOCP에 등록하는 과정을 거친다.
	// 여기서 GetSocket을 이용해서 소켓을 등록하고 해당 키는 개체의 포인터를 이용한다.
	if (!RegisterSocketToIocp(pSession->GetSocket(), reinterpret_cast<ULONG_PTR>(pSession)))
	{
		return;
	}

	// IOCP 초기 받기를 실행한다.
	if (!pSession->InitializeReadForIocp())
	{
		CZERONetworkSession*	pListen = CNetworkManager::GetInst()->GetListenSession();

		// 만일 실패하면 개체를 재시작
		pSession->Restart(pListen->GetSocket());
		return;
	}

	// 해당 세션이 접속해 있음을 설정한다.
	pSession->SetConnected(TRUE);
	INT64	iSerialNumber = CNetworkManager::GetInst()->CreateSerialNumber(SOT_USER);
	pSession->SetSerialNumber(iSerialNumber);
	cout << pSession->GetSocket() << " Client Connect" << endl;
	

}

VOID CServerIOCP::OnIoDisconnected(VOID * object)
{
	CUserSession* pSession = (CUserSession*)object;

	CNetworkManager::GetInst()->DisconnectUser(pSession);

	CZERONetworkSession*	pListen = CNetworkManager::GetInst()->GetListenSession();

	pSession->SetConnected(FALSE);
	pSession->Restart(pListen->GetSocket()); // 접속을 종료해서 개체를 다시 재시작
	cout << "현재 접속자: " << CNetworkManager::GetInst()->GetConnectCount() << endl << endl;
	cout << "Client Disconnect" << endl;
}

void CServerIOCP::UserLogin(CUserSession * pSession, BYTE * pPacket)
{
	DWORD	dwLength = 0;

	CZEROStreamSP	stream;
	stream->SetBuffer(pPacket);

	char strID[ID_SIZE] = {};
	char strPass[PASS_SIZE] = {};

	stream->ReadDWORD(&dwLength);
	stream->ReadBytes((BYTE*)strID, dwLength);
	stream->ReadDWORD(&dwLength);
	stream->ReadBytes((BYTE*)strPass, dwLength);

	BYTE	SendPacket[PACKET_SIZE] = {};

	if (!CNetworkManager::GetInst()->AddLoginUser(pSession, strID, strPass))
	{
		pSession->WritePacket(PH_LOGIN_FAILED, SendPacket, 0);
		pSession->WriteComplete();
		return;
	}

	CDx11GameObject* pUserObj = CDx11GameObject::Create("PlayerObj");
	CDx11Transform*	pTransform = pUserObj->GetTransform();
	if (CNetworkManager::GetInst()->GetConnectCount() == 2)
	{
		pTransform->SetWorldPos(73.f, 0.f, 45.f);
		cout << "초기 위치 전송" << endl;
		cout << "x: " << pTransform->GetWorldPos().x << ", y: " << pTransform->GetWorldPos().y << ", z: " << pTransform->GetWorldPos().z << endl;
	}

	if (CNetworkManager::GetInst()->GetConnectCount() == 1)
	{
		pTransform->SetWorldPos(45.f, 0.f, 96.f);
		cout << "초기 위치 전송" << endl;
		cout << "x: " << pTransform->GetWorldPos().x << ", y: " << pTransform->GetWorldPos().y << ", z: " << pTransform->GetWorldPos().z << endl;
	}

	pSession->SetUserPos(pTransform->GetWorldPos());
//	cout << "x : " << pTransform->GetWorldPos().x << ", y : " << pTransform->GetWorldPos().y << ", z : " << pTransform->GetWorldPos().z << endl;
	SAFE_RELEASE(pTransform);

	GET_SINGLE(CUserManager)->AddUser(pSession->GetSerialNumber(), pUserObj);

//	CZEROStreamSP	stream;
//	stream->SetBuffer(SendPacket);
	// 접속 성공 패킷을 보내준다.
	INT64 iSerialNumber = pSession->GetSerialNumber();
	memcpy(SendPacket, &iSerialNumber, 8);
	pSession->WritePacket(PH_LOGIN_SUCCEEDED, SendPacket, 8);
	pSession->WriteComplete();

	cout << "ID: " << strID << " 접속" << endl;
	cout << "현재 접속자: " << CNetworkManager::GetInst()->GetConnectCount() << endl << endl;

	// 모든 유저에게 이 유저가 접속했다고 보내줌
	CNetworkManager::GetInst()->SendConnectAllUser(pSession, pUserObj);
}

void CServerIOCP::UserPos(CUserSession * pSession, BYTE * pPacket)
{
	Vec3	vPos = pSession->GetUserPos();
	Vec3	vScale = pSession->GetUserScale();
	Vec3	vRot = pSession->GetUserRotation();

	CZEROStreamSP	stream;
	BYTE	byPacket[PACKET_SIZE] = {};
	stream->SetBuffer(byPacket);

	for (int i = 0; i < 3; ++i)	stream->WriteFloat(vScale[i]);
	for (int i = 0; i < 3; ++i)	stream->WriteFloat(vPos[i]);
	for (int i = 0; i < 3; ++i)	stream->WriteFloat(vRot[i]);
	
	pSession->WritePacket(PH_MY_INIT, byPacket, stream->GetLength());
	pSession->WriteComplete();
}

void CServerIOCP::MonsterPos(CUserSession * pSession, BYTE * pPacket)
{
	typedef pair<Vec3, Vec3>	monsterInfo;

	vector<monsterInfo> m_vMonsterInfo;

	Vec3	vPos1{ 21, 0, 71 };		
	Vec3	vPos2{ 37.5, 0, 124 };	
	Vec3	vPos3{ 36, 0, 126 };	
	Vec3	vPos4{ 37, 0, 128 };	
	Vec3	vPos5{ 39, 0, 129 };	
	Vec3	vPos6{ 41.5, 0, 127 };	
	Vec3	vPos7{ 41, 0, 124 };	
	Vec3	vPos8{ 94, 0, 60 };		
	Vec3	vPos9{ 96, 0, 62 };		
	Vec3	vPos10{ 92, 0, 58 };	
	Vec3	vPos11{ 95.5, 0, 64 };	
	Vec3	vPos12{ 91.5, 0, 60 };	
	Vec3	vPos13{ 110, 0, 102 };	
	Vec3	vPos14{ 110, 0, 100 };	
	Vec3	vPos15{ 112, 0, 108 };	
	Vec3	vPos16{ 114, 0, 104 };	
	Vec3	vPos17{ 108, 0, 98 };	
	Vec3	vPos18{ 106, 0, 96 };	
	Vec3	vPos19{ 107, 0, 105 };	
	Vec3	vPos20{ 112, 0, 96 };	
	Vec3	vPos21{ 117, 0, 100 };	
	Vec3	vPos22{ 100, 0, 101 };	

	Vec3	vRot1{ 0, 90, 0 };		
	Vec3	vRot2{ 0, 45, 0 };		
	Vec3	vRot3{ 0, 90, 0 };		
	Vec3	vRot4{ 0, 135, 0 };		
	Vec3	vRot5{ 0, 180, 0 };		
	Vec3	vRot6{ 0, 225, 0 };		
	Vec3	vRot7{ 0, 315, 0 };		
	Vec3	vRot8{ 0, 45, 0 };		
	Vec3	vRot9{ 0, 270, 0 };		
	Vec3	vRot10{ 0, 315, 0 };	
	Vec3	vRot11{ 0, 10, 0 };		
	Vec3	vRot12{ 0, 220, 0 };	
	Vec3	vRot13{ 0, 10, 0 };		
	Vec3	vRot14{ 0, 300, 0 };	
	Vec3	vRot15{ 0, 150, 0 };	
	Vec3	vRot16{ 0, 240, 0 };	
	Vec3	vRot17{ 0, 315, 0 };	
	Vec3	vRot18{ 0, 270, 0 };	
	Vec3	vRot19{ 0, 270, 0 };	
	Vec3	vRot20{ 0, 270, 0 };	
	Vec3	vRot21{ 0, 180, 0 };	
	Vec3	vRot22{ 0, 225, 0 };	
	
	m_vMonsterInfo.push_back(make_pair(vPos1, vRot1));
	m_vMonsterInfo.push_back(make_pair(vPos2, vRot2));
	m_vMonsterInfo.push_back(make_pair(vPos3, vRot3));
	m_vMonsterInfo.push_back(make_pair(vPos4, vRot4));
	m_vMonsterInfo.push_back(make_pair(vPos5, vRot5));
	m_vMonsterInfo.push_back(make_pair(vPos6, vRot6));
	m_vMonsterInfo.push_back(make_pair(vPos7, vRot7));
	m_vMonsterInfo.push_back(make_pair(vPos8, vRot8));
	m_vMonsterInfo.push_back(make_pair(vPos9, vRot9));
	m_vMonsterInfo.push_back(make_pair(vPos10, vRot10));
	m_vMonsterInfo.push_back(make_pair(vPos11, vRot11));
	m_vMonsterInfo.push_back(make_pair(vPos12, vRot12));
	m_vMonsterInfo.push_back(make_pair(vPos13, vRot13));
	m_vMonsterInfo.push_back(make_pair(vPos14, vRot14));
	m_vMonsterInfo.push_back(make_pair(vPos15, vRot15));
	m_vMonsterInfo.push_back(make_pair(vPos16, vRot16));
	m_vMonsterInfo.push_back(make_pair(vPos17, vRot17));
	m_vMonsterInfo.push_back(make_pair(vPos18, vRot18));
	m_vMonsterInfo.push_back(make_pair(vPos19, vRot19));
	m_vMonsterInfo.push_back(make_pair(vPos20, vRot20));
	m_vMonsterInfo.push_back(make_pair(vPos21, vRot21));
	m_vMonsterInfo.push_back(make_pair(vPos22, vRot22));
	

//	/*if (!CMonsterManager::GetInst()->Init())	return;
//	list<monsterInfo> m_MonsterInfo = CMonsterManager::GetInst()->getMonsterList();
//
//	list<monsterInfo>::iterator	InfoIter = m_MonsterInfo.begin();
//	list<monsterInfo>::iterator	InfoIterEnd = m_MonsterInfo.end();*/

	vector<monsterInfo>::iterator	InfoIter = m_vMonsterInfo.begin();
	vector<monsterInfo>::iterator	InfoIterEnd = m_vMonsterInfo.end();

	CZEROStreamSP	stream;
	BYTE	byPacket[PACKET_SIZE] = {};
	stream->SetBuffer(byPacket);

	stream->WriteInt32(m_vMonsterInfo.size());

//	cout << m_vMonsterInfo.size() << endl;
	
	for (auto p = InfoIter; p != InfoIterEnd; ++p)
	{
		stream->WriteFloat(p->first.x);	
		stream->WriteFloat(p->first.y); 
		stream->WriteFloat(p->first.z); 

		stream->WriteFloat(p->second.x);
		stream->WriteFloat(p->second.y);
		stream->WriteFloat(p->second.z);
	}
////	for (int i = 0; i < 3; ++i)	stream->WriteFloat(vPos[i]);
////	for (int i = 0; i < 3; ++i) stream->WriteFloat(vRot[i]);
//
////	cout << "몬스터 좌표" << endl;
//
////	cout << vPos.x << ", " << vPos.y << ", " << vPos.z << endl;
////	cout << vRot.x << ", " << vRot.y << ", " << vRot.z << endl;

	pSession->WritePacket(PH_MONSTER_POS, byPacket, stream->GetLength());
	pSession->WriteComplete();
}

void CServerIOCP::UserMove(CUserSession * pSession, BYTE * pPacket)
{
	CDx11GameObject*	pUser = GET_SINGLE(CUserManager)->FindUser(pSession->GetSerialNumber());
	CDx11Transform*	pTr = pUser->GetTransform();

	SAFE_RELEASE(pUser);
	SAFE_RELEASE(pTr);

	CZEROStreamSP	stream;
	Vec3	vPos;
	stream->SetBuffer(pPacket);
	stream->ReadFloat(&vPos.x);
	stream->ReadFloat(&vPos.y);
	stream->ReadFloat(&vPos.z);

	cout << pSession->GetSerialNumber() << " Client Move" << endl; // 나의 위치정보를 출력
	cout << "x : " << vPos.x << ", y : " << vPos.y << ", z : " << vPos.z << endl;

	// 다른 유저에게 내가 움직였다는 것을 보내야함
	CNetworkManager::GetInst()->SendMoveAllUser(pSession, vPos);
}

void CServerIOCP::UserRotate(CUserSession * pSession, BYTE * pPacket)
{
	CDx11GameObject* pUser = GET_SINGLE(CUserManager)->FindUser(pSession->GetSerialNumber());
	CDx11Transform* pTr = pUser->GetTransform();

	SAFE_RELEASE(pUser);
	SAFE_RELEASE(pTr);

	CZEROStreamSP	stream;
	Vec3	vRot;
	stream->SetBuffer(pPacket);
	stream->ReadFloat(&vRot.x);
	stream->ReadFloat(&vRot.y);
	stream->ReadFloat(&vRot.z);

	cout << pSession->GetSerialNumber() << " Client Rotate" << endl;

	// 다른 유저에게 내가 회전했다는 것을 보내야함
	CNetworkManager::GetInst()->SendRotAllUser(pSession, vRot);
}

void CServerIOCP::UserHeadRotate(CUserSession * pSession, BYTE * pPacket)
{
	CDx11GameObject* pUser = GET_SINGLE(CUserManager)->FindUser(pSession->GetSerialNumber());
	CDx11Transform* pTr = pUser->GetTransform();

	SAFE_RELEASE(pUser);
	SAFE_RELEASE(pTr);

	CZEROStreamSP	stream;
	Vec3	vRot;
	stream->SetBuffer(pPacket);
	stream->ReadFloat(&vRot.x);
	stream->ReadFloat(&vRot.y);
	stream->ReadFloat(&vRot.z);

//	cout << vRot.x << ", " << vRot.y << ", " << vRot.z << endl;

//	cout << pSession->GetSerialNumber() << " Client Head Rotate" << endl;

	// 다른 유저에게 나의 머리가 회전했다는 것을 보냄
	CNetworkManager::GetInst()->SendHeadRotAllUSer(pSession, vRot);
}

void CServerIOCP::UserShoot(CUserSession * pSession, BYTE * pPacket)
{
	CDx11GameObject* pUser = GET_SINGLE(CUserManager)->FindUser(pSession->GetSerialNumber());
	CDx11Transform* pTr = pUser->GetTransform();

	SAFE_RELEASE(pUser);
	SAFE_RELEASE(pTr);

	CZEROStreamSP	stream;
	Vec3	vDir;
	
	stream->SetBuffer(pPacket);

	stream->ReadFloat(&vDir.x);
	stream->ReadFloat(&vDir.y);
	stream->ReadFloat(&vDir.z);

	// cout << vDir.x << ", " << vDir.y << ", " << vDir.z << endl;
	// 다른 유저에게 내가 쐈다는것을 보냄
	CNetworkManager::GetInst()->SendShootAllUser(pSession, vDir);
}
