#pragma once
#include "../Include.h"

class CCrypt
{
public:
	static BOOL Encrypt(BYTE* source, BYTE* destination, DWORD length);
	static BOOL Decrypt(BYTE* source, BYTE* destination, DWORD length);
};

// 간단한 비트 연산을 통한 암호화 방법