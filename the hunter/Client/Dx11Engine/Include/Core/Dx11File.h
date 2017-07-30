#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

class DX11_DLL CDx11File:
	public CDx11Object
{
public:
	CDx11File();
	CDx11File(const char* pFileName, char* pMode);
	~CDx11File();

private:
	FILE*			m_pFile;
	unsigned char*	m_pData;
	UINT			m_iFileSize;
	UINT			m_iCursor;

public:
	UINT GetFileSize()	const;
	UINT GetCursor()	const;

public:
	bool Open(const char* pFileName, char* pMode);
	bool IsOpen();
	void Read(void* pData, UINT iSize);
	void Write(void* pData, UINT iSize);
};

DX11_END
