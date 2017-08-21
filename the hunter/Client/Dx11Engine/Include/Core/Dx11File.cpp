#include "Dx11File.h"

DX11_USING

CDx11File::CDx11File()	:
	m_pFile(NULL),
	m_pData(NULL),
	m_iFileSize(0),
	m_iCursor(0)
{
}

CDx11File::CDx11File(const char * pFileName, char * pMode) :
	m_pFile(NULL),
	m_pData(NULL),
	m_iFileSize(0),
	m_iCursor(0)
{
	Open(pFileName, pMode);
}

CDx11File::~CDx11File()
{
	if (m_pFile)
	{
		SAFE_DELETE_ARRAY(m_pData);
		fclose(m_pFile);
	}
}

UINT CDx11File::GetFileSize() const
{
	return m_iFileSize;
}

UINT CDx11File::GetCursor() const
{
	return m_iCursor;
}

bool CDx11File::Open(const char * pFileName, char * pMode)
{
	if (m_pFile)
	{
		SAFE_DELETE_ARRAY(m_pData);
		fclose(m_pFile);
	}

	fopen_s(&m_pFile, pFileName, pMode);

	if (!m_pFile)
		return false;

	fseek(m_pFile, 0, SEEK_END);
	m_iFileSize = ftell(m_pFile);
	fseek(m_pFile, 0, SEEK_SET);

	if (pMode[0] == 'r')
	{
		m_pData = new unsigned char[m_iFileSize];
		fread(m_pData, m_iFileSize, 1, m_pFile);
	}

	return true;
}

bool CDx11File::IsOpen()
{
	return m_pFile != NULL;
}

void CDx11File::Read(void * pData, UINT iSize)
{
	if (!m_pData)
		return;

	else if (m_iCursor + iSize > m_iFileSize)
		return;

	memcpy(pData, m_pData + m_iCursor, iSize);
	m_iCursor += iSize;
}

void CDx11File::Write(void * pData, UINT iSize)
{
	if (m_pData)
		return;

	fwrite(pData, iSize, 1, m_pFile);
	m_iCursor += iSize;
	m_iFileSize += iSize;
}
