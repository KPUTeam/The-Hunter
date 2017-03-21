#pragma once
#include "../Include.h"
#include "../../System Library/MemoryPool/MemoryPool.h"

class CStream : public CMemoryPool<CStream>
{
public:
	CStream();
	~CStream();

private:
	BYTE *mBufferPointer;
	DWORD mLength;

public:
	BOOL SetBuffer();
	BOOL SetBuffer(BYTE* buffer);

	BOOL ReadInt32(INT* data);
	BOOL ReadDWORD(DWORD* data);
	BOOL ReadDWORD_PTR(DWORD_PTR* data);
	BOOL ReadByte(BYTE* data);
	BOOL ReadBytes(BYTE* data, DWORD length);	
	BOOL ReadFloat(FLOAT* data);
	BOOL ReadInt64(INT64* data);
	BOOL ReadUSHORT(USHORT* data);
	BOOL ReadSHORT(SHORT* data);
	BOOL ReadBOOL(BOOL* data);
	BOOL ReadWCHAR(WCHAR* data);
	BOOL ReadWCHARs(LPWSTR data, DWORD length);

	BOOL WriteInt32(INT data);
	BOOL WriteDWORD(DWORD data);
	BOOL WriteDWORD_PTR(DWORD_PTR data);
	BOOL WriteByte(BYTE data);
	BOOL WriteBytes(BYTE* data, DWORD length);
	BOOL WriteFloat(FLOAT data);
	BOOL WriteInt64(INT64 data);
	BOOL WriteUSHORT(USHORT data);
	BOOL WriteSHORT(SHORT data);
	BOOL WriteBOOL(BOOL data);
	BOOL WriteWCHAR(WCHAR data);
	BOOL WriteWCHARs(LPCWSTR data, DWORD length);

	DWORD GetLength();
};

