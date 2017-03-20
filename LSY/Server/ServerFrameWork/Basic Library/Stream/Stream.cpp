#include "Stream.h"



CStream::CStream()
{
	mBufferPointer = NULL;
	mLength = 0;
}


CStream::~CStream()
{
}

BOOL CStream::SetBuffer()
{
	if (!mBufferPointer)
		return FALSE;

	mLength = 0;
	
	return TRUE;
}

// 우선 버퍼를 등록하는 함수로 멤버 변수인 mBufferPointer에 버퍼의 포인터를 저장한다.
// 이때 전체 데이터 길이를 관리하는 mLength 변수를 초기화한다.
BOOL CStream::SetBuffer(BYTE * buffer)
{
	if (!buffer)
		return false;

	mBufferPointer = buffer;
	mLength = 0;

	return 0;
}

BOOL CStream::ReadInt32(INT * data) // INT32형 데이터를 읽을 때 사용한다.
{
	// CopyMemory를 이용해서 지정된 버퍼에 값을 복사한다.
	memcpy(data, mBufferPointer + mLength, sizeof(INT));

	// 전체 길이를 데이터의 길이만큼 더해준다.
	mLength += sizeof(INT);

	return TRUE;
}

BOOL CStream::ReadDWORD(DWORD * data)
{
	memcpy(data, mBufferPointer + mLength, sizeof(DWORD));

	mLength += sizeof(DWORD);

	return 0;
}

BOOL CStream::ReadDWORD_PTR(DWORD_PTR * data)
{
	memcpy(data, mBufferPointer + mLength, sizeof(DWORD_PTR));

	mLength += sizeof(DWORD_PTR);

	return 0;
}

// BYTE형 데이터를 Call by reference를 이용해서 받아온다.
BOOL CStream::ReadByte(BYTE * data)
{
	// 값을 받아올 data 변수에 값을 복사한다.
	memcpy(data, mBufferPointer + mLength, sizeof(BYTE));

	// 복사된 값만큼 길이를 더한다. 여기서는 BYTE형이기 때문에 BYTE형만큼의 길이를 더한다.
	mLength += sizeof(BYTE);

	return TRUE;
}

BOOL CStream::ReadBytes(BYTE * data, DWORD length)
{
	memcpy(data, mBufferPointer + mLength, length);

	mLength += length;

	return TRUE;
}

BOOL CStream::ReadFloat(FLOAT * data)
{
	memcpy(data, mBufferPointer + mLength, sizeof(FLOAT));

	mLength += sizeof(FLOAT);

	return TRUE;
}

BOOL CStream::ReadInt64(INT64 * data)
{
	memcpy(data, mBufferPointer + mLength, sizeof(INT64));

	mLength += sizeof(INT64);

	return TRUE;
}

BOOL CStream::ReadUSHORT(USHORT * data)
{
	memcpy(data, mBufferPointer + mLength, sizeof(INT64));

	mLength += sizeof(INT64);

	return TRUE;
}

BOOL CStream::ReadSHORT(SHORT * data)
{
	memcpy(data, mBufferPointer + mLength, sizeof(SHORT));

	mLength += sizeof(SHORT);

	return TRUE;
}

BOOL CStream::ReadBOOL(BOOL * data)
{
	memcpy(data, mBufferPointer + mLength, sizeof(BOOL));

	mLength += sizeof(BOOL);

	return TRUE;
}

BOOL CStream::ReadWCHAR(WCHAR * data)
{
	memcpy(data, mBufferPointer + mLength, sizeof(WCHAR));

	mLength += sizeof(WCHAR);

	return 0;
}

BOOL CStream::ReadWCHARs(LPWSTR data, DWORD length)
{
	memcpy(data, mBufferPointer + mLength, length + sizeof(LPWSTR));

	mLength += length * sizeof(LPWSTR);

	return 0;
}

BOOL CStream::WriteInt32(INT data) // INT32형 데이터를 쓸 때 사용한다.
{
	memcpy(mBufferPointer + mLength, &data, sizeof(INT));

	mLength += sizeof(INT);

	return TRUE;
}

BOOL CStream::WriteDWORD(DWORD data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(DWORD));

	mLength += sizeof(DWORD);

	return TRUE;
}

BOOL CStream::WriteDWORD_PTR(DWORD_PTR data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(DWORD_PTR));

	mLength += sizeof(DWORD_PTR);

	return TRUE;
}

BOOL CStream::WriteByte(BYTE data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(BYTE));

	mLength += sizeof(BYTE);

	return TRUE;
}

BOOL CStream::WriteBytes(BYTE * data, DWORD length)
{
	memcpy(mBufferPointer + mLength, data, length);
	
	mLength += length;

	return TRUE;
}

BOOL CStream::WriteFloat(FLOAT data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(FLOAT));

	mLength += sizeof(FLOAT);

	return TRUE;
}

BOOL CStream::WriteInt64(INT64 data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(INT64));

	mLength += sizeof(INT64);

	return TRUE;
}

BOOL CStream::WriteUSHORT(USHORT data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(USHORT));

	mLength += sizeof(USHORT);

	return TRUE;
}

BOOL CStream::WriteSHORT(SHORT data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(SHORT));

	mLength += sizeof(SHORT);

	return TRUE;
}

BOOL CStream::WriteBOOL(BOOL data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(BOOL));

	mLength += sizeof(BOOL);

	return TRUE;
}

BOOL CStream::WriteWCHAR(WCHAR data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(WCHAR));

	mLength += sizeof(WCHAR);

	return TRUE;
}

BOOL CStream::WriteWCHARs(LPCWSTR data, DWORD length)
{
	memcpy(mBufferPointer + mLength, &data, length * sizeof(LPCWSTR));

	mLength += length * sizeof(LPCWSTR);

	return 0;
}

DWORD CStream::GetLength()
{
	return mLength;
}
