#include "stdafx.h"
#include "LoginNetwork.h"
#include "NetworkManager.h"
#include "Mainfrm.h"
#include "LoginForm.h"

#define		GAMESERVER_PORT	9001
//#define		GAMESERVER_IP	"127.0.0.1"

extern CString ipNumber;

CLoginNetwork::CLoginNetwork()
{
}


CLoginNetwork::~CLoginNetwork()
{
}

void CLoginNetwork::SetFrame(CMainFrame * pFrame)
{
	m_pMainFrame = pFrame;
}

void CLoginNetwork::OnIoRead()
{
	SOCKET	hSocket = CNetworkManager::GetInst()->GetSocket();
	PACKET  tPacket = {};

	recv(hSocket, (char*)&tPacket, PACKET_SIZE + 8, 0);

	switch (tPacket.eHeader)
	{
	case PH_LOGIN:
	{
		int iSuccess = 0;
		int iReadSize = 0;

		memcpy(&iSuccess, tPacket.strPacket, 4);
		iReadSize += 4;

		char strID[ID_SIZE] = {};
		memcpy(strID, tPacket.strPacket + iReadSize, sizeof(strID));
		iReadSize += sizeof(strID);

		char strPass[PASS_SIZE] = {};
		memcpy(strPass, tPacket.strPacket + iReadSize, sizeof(strPass));

		if (iSuccess == 1)
		{
			AfxMessageBox("로그인 성공");
			STARTUPINFO	tInfo = {};
			PROCESS_INFORMATION tProcessInfo = {};
			tInfo.cb = sizeof(STARTUPINFO);

			// 로그인 정보 파일을 만든다.
			FILE*	pFile = NULL;
			fopen_s(&pFile, "LoginInfo.lif", "wb");

			if (pFile)
			{
				char	strIP[16] = {};
				strcpy_s(strIP, ipNumber);
				short	sPort = GAMESERVER_PORT;
				fwrite(strIP, 16, 1, pFile);
				fwrite(&sPort, 2, 1, pFile);
				fwrite(strID, sizeof(strID), 1, pFile);
				fwrite(strPass, sizeof(strPass), 1, pFile);
				fclose(pFile);
			}
#ifdef _DEBUG
			::CreateProcess(_TEXT("../../Client/Bin_Debug/Client_Debug.exe"), 0, 0, 0, FALSE, 0, 0, 0, &tInfo, &tProcessInfo);
#else
			::CreateProcess(_TEXT("../../Client/Bin/Client.exe"), 0, 0, 0, FALSE, 0, 0, 0, &tInfo, &tProcessInfo);
#endif // _DEBUG
			m_pMainFrame->SendMessage(WM_CLOSE);
		}
	}
	break;

	case PH_JOIN:
		int iSuccess = 0;
		memcpy(&iSuccess, tPacket.strPacket, 4);

		if (iSuccess == 1)
		{
			AfxMessageBox(_TEXT("회원가입 성공"));
		}

		else
		{
			AfxMessageBox(_TEXT("회원가입 실패"));
		}

		break;
	}
}

void CLoginNetwork::OnIoConnected()
{
}

void CLoginNetwork::OnIoDisconnected()
{
}
