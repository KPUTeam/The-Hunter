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
	OVERLAPPED	Overlapped; // 원래 구조체
	IO_TYPE		IoType;    // 이 구조체가 하는 일
						   // IO_READ: 읽기 작업을 할 때 사용
						   // IO_WRITE: 쓰기 작업을 할 때 사용
						   // IO_ACCEPT: Accept를 받았을 때 사용
	void		*Object;   // 부모 개체 주소
} OVERLAPPED_EX, *POVERLAPPED_EX;

// Winsock의 WSASend, WSARecv등의 함수를 사용하려면 Overlapped라는 구조체를 사용해야 한다.
