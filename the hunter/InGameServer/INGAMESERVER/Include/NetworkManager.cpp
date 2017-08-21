#include "NetworkManager.h"
#include "UserSession.h"
#include "UserManager.h"
#include "Logic.h"
#include "Network\ZEROStream.h"
#include "GameObject/Dx11GameObject.h"
#include "Component/Dx11Transform.h"

DX11_USING

CNetworkManager* CNetworkManager::m_pInst = NULL;

CNetworkManager::CNetworkManager()
	: m_pLogic(NULL)
{
	WSADATA	wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
}


CNetworkManager::~CNetworkManager()
{
	CUserManager::DestroyInst();

	for (size_t i = 0; i < m_vecConnectSession.size(); ++i)
	{
		m_vecConnectSession[i]->End();
		delete m_vecConnectSession[i];
	}

	m_vecConnectSession.clear();
	m_pIOCP->End();
	delete m_pIOCP;

	WSACleanup();
}

bool CNetworkManager::Init(short sPort)
{
	// Initialize UserManager
	if (!CUserManager::GetInst()->Init())
		return false;

	// ��� ������ �о�´�.
	FILE* pFile = NULL;
	fopen_s(&pFile, "Member.mbr", "rb");

	if (pFile)
	{
		int iMemberCount = 0;
		fread(&iMemberCount, 4, 1, pFile);

		for (int i = 0; i < iMemberCount; ++i)
		{
			int iLength = 0;
			fread(&iLength, 4, 1, pFile);
			
			char strID[ID_SIZE] = {};
			char strPass[PASS_SIZE] = {};

			fread(strID, iLength, 1, pFile);

			fread(&iLength, 4, 1, pFile);
			fread(strPass, iLength, 1, pFile);

			PMEMBER pMember = new MEMBER;
			strcpy_s(pMember->strID, strID);
			strcpy_s(pMember->strPass, strPass);

			m_mapMember.insert(make_pair(strID, pMember));
		}

		fclose(pFile);
	}

	// IOCP�� ����
	m_pIOCP = new CServerIOCP;
	if (!m_pIOCP->Begin())
		return false;

	m_pListenSession = new CZERONetworkSession;
	if (!m_pListenSession->Begin())
	{
		m_pIOCP->End();
		return false;
	}

	// TCP�� �����
	if (!m_pListenSession->TcpBind())
	{
		m_pIOCP->End();
		return false;
	}

	// ��Ʈ�� ����
	if (!m_pListenSession->Listen(PORT, MAX_USER))
	{
		m_pIOCP->End();
		return false;
	}

	// Listen socket�� IOCP�� ���
	if (!m_pIOCP->RegisterSocketToIocp(m_pListenSession->GetSocket(), (ULONG_PTR)m_pListenSession))
	{
		m_pIOCP->End();
		return false;
	}

	// ���� ����ŭ accept ó��
	for (int i = 0; i < MAX_USER; ++i)
	{
		CUserSession* pSesison = new CUserSession;
		m_vecConnectSession.push_back(pSesison);
	}

	for (int i = 0; i < MAX_USER; ++i)
	{
		if (!m_vecConnectSession[i]->Begin())
			return false;

		if (!m_vecConnectSession[i]->Accept(m_pListenSession->GetSocket()))
			return false;
	}

	m_pLogic = new CLogic;
	m_pLogic->Init();

	return true;
}

void CNetworkManager::Run()
{
	while (true)
	{
		getchar();
		break;
	}
}

CZERONetworkSession * CNetworkManager::GetListenSession() const
{
	return m_pListenSession;
}

size_t CNetworkManager::GetConnectCount() const
{
	return m_mapLoginUser.size();
}

bool CNetworkManager::AddLoginUser(CUserSession * pUser, char * pID, char * pPass)
{
	if (FindUser(pUser->GetSocket()))
		return false;

	PMEMBER	pMember = FindMember(pID);
	if (!pMember)
		return false;
	else if (strcmp(pMember->strPass, pPass) != 0)
		return false;

	m_mapLoginUser.insert(make_pair(pUser->GetSocket(), pUser));
	cout << "�α��� ���� �߰�" << endl;
	return true;
}

bool CNetworkManager::DisconnectUser(CUserSession * pUser)
{
	unordered_map<SOCKET, class CUserSession*>::iterator	iter = m_mapLoginUser.find(pUser->GetSocket());

	if (iter == m_mapLoginUser.end())
		return false;
	
	m_mapLoginUser.erase(iter);
	return true;
}

CUserSession * CNetworkManager::FindUser(SOCKET hSocket)
{
	unordered_map<SOCKET, class CUserSession*>::iterator	iter = m_mapLoginUser.find(hSocket);

	if (iter == m_mapLoginUser.end())
		return NULL;

	return iter->second;
}

void CNetworkManager::SendConnectAllUser(CUserSession * pUser, CDx11GameObject * pObj)
{
	unordered_map<SOCKET, class CUserSession*>::iterator	iter;
	unordered_map<SOCKET, class CUserSession*>::iterator	iterEnd = m_mapLoginUser.end();

	for (iter = m_mapLoginUser.begin(); iter != iterEnd; ++iter)
	{
		if (iter->second == pUser)	continue;

		CZEROStreamSP	stream1;
		BYTE	byPacket[PACKET_SIZE] = {};
		stream1->SetBuffer(byPacket);
		INT64	iSerialNumber = pUser->GetSerialNumber();	// �� session�� serialnumber
		stream1->WriteInt64(iSerialNumber);		// serialnumber�� ����
		CDx11Transform* pTr = pObj->GetTransform();	// �� gameobject�� transform�� ����

		Vec3	vPos = pTr->GetWorldPos();
		Vec3	vRot = pTr->GetWorldRot();
		Vec3	vScale = pTr->GetWorldScale();

		for (int i = 0; i < 3; ++i)
		{
			stream1->WriteFloat(vScale[i]);
		}

		for (int i = 0; i < 3; ++i)
		{
			stream1->WriteFloat(vRot[i]);
		}

		for (int i = 0; i < 3; ++i)
		{
			stream1->WriteFloat(vPos[i]);
		}

		// ������ �������ִ� �����鿡�� ���� ������ ����... serialnumber��簡 ��ǥ���
		iter->second->WritePacket(PH_USER_LOGIN, byPacket, stream1->GetLength());
		iter->second->WriteComplete();
		cout << iter->second->GetSerialNumber() << " client���� ���� ������ ����" << endl;
		SAFE_RELEASE(pTr);

		// ��� �������� ������ ������ Ŭ���̾�Ʈ���� �����ش�.
		iSerialNumber = iter->second->GetSerialNumber();

		// ���� ������Ʈ�� ���´�.
		CDx11GameObject*	pUserObj = GET_SINGLE(CUserManager)->FindUser(iSerialNumber);
		CDx11Transform*	pTransform = pUserObj->GetTransform();

		vPos = pTransform->GetWorldPos();
		vRot = pTransform->GetWorldRot();
		vScale = pTransform->GetWorldScale();
		
		CZEROStreamSP	stream;
		stream->SetBuffer(byPacket);
		stream->WriteInt64(iSerialNumber);

		for (int i = 0; i < 3; ++i)	stream->WriteFloat(vScale[i]);
		for (int i = 0; i < 3; ++i)	stream->WriteFloat(vRot[i]);
		for (int i = 0; i < 3; ++i)	stream->WriteFloat(vPos[i]);

		pUser->WritePacket(PH_USER_LOGIN, byPacket, stream->GetLength());
		SAFE_RELEASE(pTransform);
	}

}

void CNetworkManager::SendMoveAllUser(CUserSession * pUser, Vec3 vPos)
{
	unordered_map<SOCKET, class CUserSession*>::iterator	iter;
	unordered_map<SOCKET, class CUserSession*>::iterator	iterEnd = m_mapLoginUser.end();

	for (iter = m_mapLoginUser.begin(); iter != iterEnd; ++iter)
	{
		if (iter->second == pUser)	continue;

		CZEROStreamSP	stream;
		BYTE myMovePacket[PACKET_SIZE] = {};
		stream->SetBuffer(myMovePacket);

		Vec3 Pos = vPos;

		for (int i = 0; i < 3; ++i)	stream->WriteFloat(Pos[i]);

		iter->second->WritePacket(PH_MY_MOVE_SUCCEEDED, myMovePacket, stream->GetLength());
		iter->second->WriteComplete();
	//	cout << iter->second->GetSerialNumber() << " ���� ���� ��ġ ������ ������" << endl;
	//	cout << "���� ��ġ ���� ��" << endl;

	}
}

void CNetworkManager::SendRotAllUser(CUserSession * pUser, Vec3 vRot)
{
	unordered_map<SOCKET, class CUserSession*>::iterator	iter;
	unordered_map<SOCKET, class CUserSession*>::iterator	iterEnd = m_mapLoginUser.end();

	for (iter = m_mapLoginUser.begin(); iter != iterEnd; ++iter)
	{
		if (iter->second == pUser) continue;

		CZEROStreamSP	stream;
		BYTE myRotatePacket[PACKET_SIZE] = {};
		stream->SetBuffer(myRotatePacket);

		Vec3 Rot = vRot;

		for (int i = 0; i < 3; ++i)	stream->WriteFloat(Rot[i]);

		iter->second->WritePacket(PH_MY_ROTATE_SUCCEEDED, myRotatePacket, stream->GetLength());
		iter->second->WriteComplete();

	//	cout << "���� ȸ�� ���� ��" << endl;
	}
}

void CNetworkManager::SendHeadRotAllUSer(CUserSession * pUser, Vec3 vRot)
{
	unordered_map<SOCKET, class CUserSession*>::iterator	iter;
	unordered_map<SOCKET, class CUserSession*>::iterator	iterEnd = m_mapLoginUser.end();

	for (iter = m_mapLoginUser.begin(); iter != iterEnd; ++iter)
	{
		if (iter->second == pUser)	continue;

		CZEROStreamSP	stream;
		BYTE myHeadRotatePacket[PACKET_SIZE] = {};
		stream->SetBuffer(myHeadRotatePacket);

		Vec3 Rot = vRot;
		
		for (int i = 0; i < 3; ++i)	stream->WriteFloat(Rot[i]);

		iter->second->WritePacket(PH_MY_HEAD_ROTATE_SUCCEEDED, myHeadRotatePacket, stream->GetLength());
		iter->second->WriteComplete();

//		cout << "���� �Ӹ� ȸ�� ���� ��" << endl;
	}
}

void CNetworkManager::SendGameStart()
{
	unordered_map<SOCKET, class CUserSession*>::iterator	iter;
	unordered_map<SOCKET, class CUserSession*>::iterator	iterEnd = m_mapLoginUser.end();

	for (iter = m_mapLoginUser.begin(); iter != iterEnd; ++iter)
	{
		CZEROStreamSP	stream;
		BYTE gameStartPacket[PACKET_SIZE] = {};
		stream->SetBuffer(gameStartPacket);

		iter->second->WritePacket(PH_START_GAME, gameStartPacket, stream->GetLength());
		iter->second->WriteComplete();

		cout << "���� ����" << endl;
	}
}

void CNetworkManager::SendShootAllUser(CUserSession * pUser, Vec3 vDir)
{
	unordered_map<SOCKET, class CUserSession*>::iterator	iter;
	unordered_map<SOCKET, class CUserSession*>::iterator	iterEnd = m_mapLoginUser.end();

	for (iter = m_mapLoginUser.begin(); iter != iterEnd; ++iter)
	{
		if (iter->second == pUser)	continue;

		CZEROStreamSP	stream;
		BYTE shootPacket[PACKET_SIZE] = {};
		stream->SetBuffer(shootPacket);

		Vec3 Dir = vDir;

		for (int i = 0; i < 3; ++i) stream->WriteFloat(Dir[i]);

//		cout << vDir.x << ", " << vDir.y << ", " << vDir.z << endl;

		iter->second->WritePacket(PH_MY_SHOOT_SUCCEEDED, shootPacket, stream->GetLength());
		iter->second->WriteComplete();
	}
}

void CNetworkManager::SendMonsterPosAllUser(CUserSession * pUser, Vec3 vPos, Vec3 vRot)
{
	unordered_map<SOCKET, class CUserSession*>::iterator	iter;
	unordered_map<SOCKET, class CUserSession*>::iterator	iterEnd = m_mapLoginUser.end();

	for (iter = m_mapLoginUser.begin(); iter != iterEnd; ++iter)
	{
		if (iter->second == pUser)	continue;

		CZEROStreamSP	stream;
		BYTE monsterPosPacket[PACKET_SIZE] = {};
		stream->SetBuffer(monsterPosPacket);

		Vec3	Pos = vPos;
		Vec3	Rot = vRot;

		for (int i = 0; i < 3; ++i)	stream->WriteFloat(Pos[i]);
		for (int i = 0; i < 3; ++i)	stream->WriteFloat(Rot[i]);

		cout << "���� ��ǥ: " << vPos.x << ", " << vPos.y << ", " << vPos.z << endl;

		iter->second->WritePacket(PH_MONSTER_POS, monsterPosPacket, stream->GetLength());
		iter->second->WriteComplete();
	}
}

void CNetworkManager::SendMonsterPosAllUser()
{
	unordered_map<SOCKET, class CUserSession*>::iterator	iter;
	unordered_map<SOCKET, class CUserSession*>::iterator	iterEnd = m_mapLoginUser.end();

	for (iter = m_mapLoginUser.begin(); iter != iterEnd; ++iter)
	{
	//	if (iter->second == pUser)	continue;

		CZEROStreamSP	stream;
		BYTE monsterPosPacket[PACKET_SIZE] = {};
		stream->SetBuffer(monsterPosPacket);

		Vec3	vPos = { 41.5f, 0.f, 120.f };
		Vec3	vRot = { 0, 225.f, 0 };

		for (int i = 0; i < 3; ++i)	stream->WriteFloat(vPos[i]);
		for (int i = 0; i < 3; ++i)	stream->WriteFloat(vRot[i]);

		cout << "���� ��ǥ: " << vPos.x << ", " << vPos.y << ", " << vPos.z << endl;

		iter->second->WritePacket(PH_MONSTER_POS, monsterPosPacket, stream->GetLength());
		iter->second->WriteComplete();
	}
}

void CNetworkManager::SendUserResult(CUserSession * pUser)
{
	unordered_map<SOCKET, class CUserSession*>::iterator	iter;
	unordered_map<SOCKET, class CUserSession*>::iterator	iterEnd = m_mapLoginUser.end();

	for (iter = m_mapLoginUser.begin(); iter != iterEnd; ++iter)
	{
		// ���� HP�� 0�̹Ƿ� �����״� lose ��Ŷ�� ����
		if (iter->second == pUser)
		{
			CZEROStreamSP	stream;
			BYTE	losePacket[PACKET_SIZE] = {};
			stream->SetBuffer(losePacket);

	//		cout << iter->second->GetSerialNumber() << "�������� �й��� ��Ŷ��" << endl;
			iter->second->WritePacket(PH_USER_LOSE_RET, losePacket, stream->GetLength());
			iter->second->WriteComplete();
		}

		else
		{
			CZEROStreamSP	stream1;
			BYTE	winPacket[PACKET_SIZE] = {};
			stream1->SetBuffer(winPacket);

	//		cout << iter->second->GetSerialNumber() << "�������� �¸��� ��Ŷ��" << endl;
			iter->second->WritePacket(PH_USER_WIN_RET, winPacket, stream1->GetLength());
			iter->second->WriteComplete();
		}
	}
}

INT64 CNetworkManager::CreateSerialNumber(int iObjType)
{	
	// ���� �ð� ������ �޾ƿ���
	time_t	tempTime;
	time(&tempTime);

	m_iCurrentTime = (INT)tempTime;
	m_iSerialNumber = 0;

	// ���� �ð��� �� ȣ��ƴٸ� m_iSameTime�� ����!
	m_iSameTime = (m_iCurrentTime == m_iLastTime ? m_iSameTime + 1 : 0);

	// m_iSerialNumber �� ����
	// 1. 32��Ʈ �ð� ��
	m_iSerialNumber = m_iCurrentTime | m_iSerialNumber;

	// 2. 16��Ʈ �ϴ� �̵� (������Ʈ ���� �� ����)
	m_iSerialNumber = iObjType | (m_iSerialNumber << 16);

	// 3. 16��Ʈ m_iSameTime ����
	m_iSerialNumber = m_iSameTime | (m_iSerialNumber << 16);

	m_iLastTime = m_iCurrentTime;

	return m_iSerialNumber;
}

PMEMBER CNetworkManager::FindMember(const string & strID)
{
	unordered_map<string, PMEMBER>::iterator	iter = m_mapMember.find(strID);

	if (iter == m_mapMember.end())
		return NULL;

	return iter->second;
}
