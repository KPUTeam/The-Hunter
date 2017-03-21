#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <MSWSock.h>
#include <tchar.h>
#include <assert.h>
#include <crtdbg.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "Type.h"

#define MAX_QUEUE_LENGTH	500
#define MAX_BUFFER_LENGTH   4096

#pragma comment(lib, "System Library.lib")
#pragma comment(lib, "ws2_32.lib")

using namespace std;

typedef struct _OVERLAPPED_EX
{
	OVERLAPPED	Overlapped; // ���� ����ü
	IO_TYPE		IoType;    // �� ����ü�� �ϴ� ��
						   // IO_READ: �б� �۾��� �� �� ���
						   // IO_WRITE: ���� �۾��� �� �� ���
						   // IO_ACCEPT: Accept�� �޾��� �� ���
	void		*Object;   // �θ� ��ü �ּ�
} OVERLAPPED_EX, *POVERLAPPED_EX;

// Winsock�� WSASend, WSARecv���� �Լ��� ����Ϸ��� Overlapped��� ����ü�� ����ؾ� �Ѵ�.
