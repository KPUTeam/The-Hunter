#include "Thread.h"
#include "Iocp.h"
#include "NetSession.h"
#include "NetworkManager.h"

CThread::CThread() :
	m_pIocp(NULL)
{
}

CThread::~CThread()
{
	if (m_hThread)
	{
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
	}
}

bool CThread::Create(class CIocp* pIocp)
{
	m_pIocp = pIocp;
	m_bLoop = true;

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, CThread::ThreadFunc,
		this, 0, NULL);

	return true;
}

void CThread::Run()
{
	HANDLE		hComPort = m_pIocp->GetPort();
	CNetSession*	pSession = NULL;
	PBUFFER			pBuffer = {};
	DWORD	dwTransfer = 0;

	while (m_bLoop)
	{
		GetQueuedCompletionStatus(hComPort, &dwTransfer, (DWORD*)&pSession,
			(LPOVERLAPPED*)&pBuffer, INFINITE);

		if (!pSession)
			break;

		if (pBuffer->eMode == TM_READ)
		{
			// 접속 종료
			if (dwTransfer == 0)
			{
				//delete	pSession;
				GET_SINGLE(CNetworkManager)->DestroySession(
					pSession->GetSocket());
				continue;
			}
			PPACKET		pPacket = pSession->GetReadPacket();

			switch (pPacket->eHeader)
			{
			case PH_MSG:
				cout << pPacket->strPacket << endl;

				GET_SINGLE(CNetworkManager)->SendAllSession(pPacket);
				break;
			case PH_JOIN:
			{
				cout << "회원가입 패킷" << endl;

				int		iPacketLength = 0;
				int		iLength;

				memcpy(&iLength, pPacket->strPacket, 4);
				iPacketLength += 4;

				char	strID[ID_SIZE] = {};
				memcpy(strID, pPacket->strPacket + iPacketLength,
					iLength);
				iPacketLength += iLength;

				memcpy(&iLength, pPacket->strPacket + iPacketLength,
					4);
				iPacketLength += 4;

				char	strPass[PASS_SIZE] = {};
				memcpy(strPass, pPacket->strPacket + iPacketLength,
					iLength);
				iPacketLength += iLength;

				int		iSuccess = 1;
				if (!GET_SINGLE(CNetworkManager)->AddMember(
					strID, strPass))
				{
					iSuccess = 0;
					cout << "중복된 아이디가 있습니다." << endl;

					PACKET	tPacket = {};
					tPacket.eHeader = PH_JOIN;
					tPacket.iSize = 4;
					memcpy(tPacket.strPacket, &iSuccess, 4);

					pSession->Write(&tPacket);
				}

				else
				{
					PACKET	tPacket = {};
					tPacket.eHeader = PH_JOIN;
					tPacket.iSize = 4;
					memcpy(tPacket.strPacket, &iSuccess, 4);

					pSession->Write(&tPacket);
				}
			}
			break;

			case PH_LOGIN:
			{
				cout << "로그인 패킷" << endl;

				int		iPacketLength = 0;
				int		iLength;

				memcpy(&iLength, pPacket->strPacket, 4);
				iPacketLength += 4;

				char	strID[ID_SIZE] = {};
				memcpy(strID, pPacket->strPacket + iPacketLength,
					iLength);
				iPacketLength += iLength;

				memcpy(&iLength, pPacket->strPacket + iPacketLength,
					4);
				iPacketLength += 4;

				char	strPass[PASS_SIZE] = {};
				memcpy(strPass, pPacket->strPacket + iPacketLength,
					iLength);
				iPacketLength += iLength;

				PMEMBER	pMember = GET_SINGLE(CNetworkManager)->FindMember(strID);

				int		iSuccess = 0;
				if (pMember)
				{
					if (pMember->bLogin)
					{
						iSuccess = 0;
						cout << "로그인 중복" << endl;
						break;
					}

					// 로그인 성공
					else if (strcmp(pMember->strPass, strPass) == 0)
					{
						iSuccess = 1;
						cout << strID << " 유저 로그인 성공" << endl;
						pMember->bLogin = true;
						pMember->hSocket = pSession->GetSocket();
						pSession->Login(pMember);
					}

					// 로그인 실패
					else
					{
						cout << strID << " 유저 로그인 실패" << endl;
					}
				}

				else
				{
					iSuccess = 2;
					// 등록된 계정이 없다.
					cout << "등록된 계정이 없습니다." << endl;
				}

				PACKET	tPacket = {};
				tPacket.eHeader = PH_LOGIN;
				tPacket.iSize = 4;
				memcpy(tPacket.strPacket, &iSuccess, 4);

				pSession->Write(&tPacket);
			}
			break;
			case PH_LOGOUT:
			{
				PMEMBER	pUserInfo = pSession->GetUserInfo();
				if (pUserInfo)
				{
					pUserInfo->bLogin = false;
					pUserInfo->hSocket = 0;
					pSession->Login(NULL);
				}
			}
			break;
			}

			pSession->Read();
		}

		else if (pBuffer->eMode == TM_WRITE)
		{
			cout << "Write Complete" << endl;
		}
	}
}

void CThread::End()
{
	m_bLoop = false;
}

unsigned int CThread::ThreadFunc(void * pArg)
{
	CThread*	pThread = (CThread*)pArg;

	pThread->Run();

	return 0;
}
