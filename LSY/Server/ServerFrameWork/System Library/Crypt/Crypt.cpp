#include "Crypt.h"

const INT C1  = 52845;
const INT C2  = 22719;
const INT KEY = 72957;

BOOL CCrypt::Encrypt(BYTE * source, BYTE * destination, DWORD length) // ��ȣȭ �Լ�
{
	DWORD i;
	INT Key = KEY;

	if (!source || !destination || length <= 0)
		return FALSE;

	for (i = 0; i < length; ++i)
	{
		// 1����Ʈ�� Key����ŭ ������ ���ְ� 8��Ʈ �о��ش�.
		destination[i] = source[i] ^ Key >> 8;
		// ���� ����Ʈ������ ����� Ű�� C1, C2�� �̿��ؼ� �������ش�.
		Key = (destination[i] + Key) * C1 + C2;
	}

	return TRUE;
}

BOOL CCrypt::Decrypt(BYTE * source, BYTE * destination, DWORD length) // ��ȣȭ �Լ�
{
	DWORD i;
	BYTE PreviousBlock;
	INT Key = KEY;

	if (!source || !destination || length <= 0)
		return FALSE;

	for (i = 0; i < length; ++i) // ����Ʈ ������ ��ȣȭ�� �ϰ� �ȴ�.
	{
		PreviousBlock = source[i];
		destination[i] = source[i] ^ Key >> 8;
		// ���� ����� ������ �ξ��� ��ȣȭ�� ���Ǵ� Ű�� �˾Ƴ���.
		Key = (PreviousBlock + Key) * C1 + C2;
	}

	return 0;
}
