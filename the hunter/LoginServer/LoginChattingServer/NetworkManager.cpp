#include "NetworkManager.h"
#include "NetSession.h"
#include "Iocp.h"

DEFINITION_SINGLE(CNetworkManager)

CNetworkManager::CNetworkManager()
	:m_pIocp(NULL),
	 m_pListen(NULL)
{
	WSADATA	data;
	WSAStartup(MAKEWORD(2, 2), &data);
	
	// 회원 정보 파일을 읽어온다.
	FILE*	pFile = NULL;

	fopen_s(&pFile, "Member.mbr", "rb");

	if (pFile)
	{
		int		iMemberCount = 0;
		fread(&iMemberCount, 4, 1, pFile);

		for (int i = 0; i < iMemberCount; ++i)
		{
			int		iLength = 0;
			fread(&iLength, 4, 1, pFile);
			char	strID[ID_SIZE] = {};
			char	strPass[PASS_SIZE] = {};

			fread(strID, iLength, 1, pFile);

			fread(&iLength, 4, 1, pFile);
			fread(strPass, iLength, 1, pFile);

			PMEMBER	pMember = new MEMBER;

			memset(pMember, 0, sizeof(MEMBER));

			strcpy_s(pMember->strID, strID);
			strcpy_s(pMember->strPass, strPass);

			m_mapMember.insert(make_pair(strID, pMember));
		}

		fclose(pFile);
	}

	else
	{
		fopen_s(&pFile, "Member.mbr", "wb");

		int		iCount = 0;

		fwrite(&iCount, 4, 1, pFile);

		fclose(pFile);
	}
}


CNetworkManager::~CNetworkManager()
{
	FILE*	pFile = NULL;
	fopen_s(&pFile, "Member.mbr", "wb");

	int		iCount = m_mapMember.size();

	fwrite(&iCount, 4, 1, pFile);

	unordered_map<string, PMEMBER>::iterator	iter;
	unordered_map<string, PMEMBER>::iterator	iterEnd = m_mapMember.end();

	for (iter = m_mapMember.begin(); iter != iterEnd; ++iter)
	{
		int		iLength = strlen(iter->second->strID);
		fwrite(&iLength, 4, 1, pFile);
		fwrite(iter->second->strID, iLength, 1, pFile);

		iLength = strlen(iter->second->strPass);
		fwrite(&iLength, 4, 1, pFile);
		fwrite(iter->second->strPass, iLength, 1, pFile);
	}

	fclose(pFile);

	Safe_Delete_Map(m_mapMember);
	SAFE_DELETE(m_pIocp);
	Safe_Delete_Map(m_mapSession);
	WSACleanup();
}

HANDLE CNetworkManager::GetCompletionPort() const
{
	return m_pIocp->GetPort();
}

bool CNetworkManager::DestroySession(SOCKET hSocket)
{
	unordered_map<SOCKET, class CNetSession*>::iterator	iter = m_mapSession.find(hSocket);

	if (iter == m_mapSession.end())
		return false;

	PMEMBER	pMember = iter->second->GetUserInfo();

	if (pMember)
	{
		pMember->bLogin = false;
		pMember->hSocket = 0;
	}

	SAFE_DELETE(iter->second);

	m_mapSession.erase(iter);

	return true;
}

void CNetworkManager::SendAllSession(PPACKET pPacket)
{
	unordered_map<SOCKET, class CNetSession*>::iterator	iter;
	unordered_map<SOCKET, class CNetSession*>::iterator	iterEnd = m_mapSession.end();

	for (iter = m_mapSession.begin(); iter != iterEnd; ++iter)
	{
		if (!iter->second->GetUserInfo())
			continue;

		iter->second->Write(pPacket);
	}
}

ostream& operator<<(ostream& os, string s)
{
	string::iterator iter = s.begin();
	os << *iter;
	return os;
}


bool CNetworkManager::Init()
{
	// 서버용 세션을 만들어준다.
	m_pListen = new CNetSession;

	if (!m_pListen->CreateListen())
	{
		SAFE_DELETE(m_pListen);
		return false;
	}

	m_mapSession.insert(make_pair(m_pListen->GetSocket(), m_pListen));

	m_pIocp = new CIocp;

	m_pIocp->CreateIOCP();
	
	return true;
}

void CNetworkManager::Run()
{
	while (true)
	{
		CNetSession*	pSession = m_pListen->Accept();

		m_mapSession.insert(make_pair(pSession->GetSocket(), pSession));
	}
}

bool CNetworkManager::AddMember(char * pID, char * pPass)
{
	if (FindMember(pID))
		return false;

	PMEMBER pMember = new MEMBER;

	strcpy_s(pMember->strID, pID);
	strcpy_s(pMember->strPass, pPass);

	m_mapMember.insert(make_pair(pID, pMember));

	return true;
}

PMEMBER CNetworkManager::FindMember(char * pID)
{
	unordered_map<string, PMEMBER>::iterator	iter = m_mapMember.find(pID);

	if (iter == m_mapMember.end())
		return NULL;

	return iter->second;
}
