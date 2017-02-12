#include "Dx11Object.h"

DX11_USING

CDx11Object::CDx11Object()	:
	m_iRefCount(1),
	m_bEnable(true),
	m_bAlive(true)
{
}

CDx11Object::~CDx11Object()
{
}

string CDx11Object::GetTag()
{
	return m_strTag;
}

bool CDx11Object::GetEnable()
{
	return m_bEnable;
}

bool CDx11Object::GetAlive()
{
	return m_bAlive;
}

string CDx11Object::GetTypeName()
{
	return m_strTypeName;
}

void CDx11Object::SetTag(const string & strTag)
{
	m_strTag = strTag;
}

void CDx11Object::SetEnable(bool bEnable)
{
	m_bEnable = bEnable;
}

void CDx11Object::Death()
{
	m_bAlive = false;
}

int CDx11Object::AddRef()
{
	++m_iRefCount;

	return m_iRefCount;
}

int CDx11Object::Release()
{
	--m_iRefCount;

	if (m_iRefCount == 0)
	{
		delete	this;
		return 0;
	}

	return m_iRefCount;
}
