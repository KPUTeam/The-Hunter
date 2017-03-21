#include "Crypt.h"

const INT C1  = 52845;
const INT C2  = 22719;
const INT KEY = 72957;

BOOL CCrypt::Encrypt(BYTE * source, BYTE * destination, DWORD length) // 암호화 함수
{
	DWORD i;
	INT Key = KEY;

	if (!source || !destination || length <= 0)
		return FALSE;

	for (i = 0; i < length; ++i)
	{
		// 1바이트를 Key값만큼 제곱을 해주고 8비트 밀어준다.
		destination[i] = source[i] ^ Key >> 8;
		// 다음 바이트에서는 사용할 키를 C1, C2를 이용해서 변경해준다.
		Key = (destination[i] + Key) * C1 + C2;
	}

	return TRUE;
}

BOOL CCrypt::Decrypt(BYTE * source, BYTE * destination, DWORD length) // 복호화 함수
{
	DWORD i;
	BYTE PreviousBlock;
	INT Key = KEY;

	if (!source || !destination || length <= 0)
		return FALSE;

	for (i = 0; i < length; ++i) // 바이트 단위로 암호화를 하게 된다.
	{
		PreviousBlock = source[i];
		destination[i] = source[i] ^ Key >> 8;
		// 이전 블록을 저장해 두었다 복호화에 사용되는 키를 알아낸다.
		Key = (PreviousBlock + Key) * C1 + C2;
	}

	return 0;
}
