#include "Dx11Object.h"

DX11_USING

CDx11Object::CDx11Object()	:
	m_iRef(1),
	m_bEnable(true),
	m_bAlive(true)
{
}

CDx11Object::~CDx11Object()
{
}

void CDx11Object::Remove()
{
	m_iRef = 0;
	delete	this;
}

int CDx11Object::Release()
{
	--m_iRef;

	if (m_iRef == 0)
	{
		delete	this;
		return 0;
	}
	
	return m_iRef;
}

int CDx11Object::AddRef()
{
	++m_iRef;
	
	return m_iRef;
}

void CDx11Object::SetTag(const string & strTag)
{
	m_strTag = strTag;
}

void CDx11Object::SetName(const string & strName)
{
	m_strName = strName;
}

void CDx11Object::SetEnable(bool bEnable)
{
	m_bEnable = bEnable;
}

void CDx11Object::Death()
{
	m_bAlive = false;
}

string CDx11Object::GetTag()
{
	return m_strTag;
}

string CDx11Object::GetName()
{
	return m_strName;
}

string CDx11Object::GetTypeName()
{
	return m_strTypeName;
}

bool CDx11Object::GetEnable()
{
	return m_bEnable;
}

bool CDx11Object::GetAlive()
{
	return m_bAlive;
}

int CDx11Object::GetRef()
{
	return m_iRef;
}
