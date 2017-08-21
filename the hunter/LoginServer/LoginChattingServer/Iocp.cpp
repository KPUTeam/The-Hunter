#include "Iocp.h"
#include "Thread.h"



CIocp::CIocp()
{
	m_hComPort = NULL;
}


CIocp::~CIocp()
{
	for (size_t i = 0; i < m_vecThread.size(); ++i)
	{
		m_vecThread[i]->End();
	}

	if (m_hComPort)
	{
		for (size_t i = 0; i < m_vecThread.size(); ++i)
		{
			PostQueuedCompletionStatus(m_hComPort, 0, 0, 0);
		}
		CloseHandle(m_hComPort);
	}

	Safe_Delete_VecList(m_vecThread);
}

HANDLE CIocp::GetPort() const
{
	return m_hComPort;
}

bool CIocp::CreateIOCP()
{
	m_hComPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE,
		0, 0, 0);

	// 이벤트를 체크할 스레드를 생성한다.
	SYSTEM_INFO		tSystem;
	GetSystemInfo(&tSystem);

	for (DWORD i = 0; i < tSystem.dwNumberOfProcessors; ++i)
	{
		CThread*	pThread = new CThread;
		pThread->Create(this);
		m_vecThread.push_back(pThread);
	}

	return true;
}
