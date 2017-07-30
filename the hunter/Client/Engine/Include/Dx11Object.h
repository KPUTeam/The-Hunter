#pragma once

#include "Dx11Value.h"

DX11_BEGIN

class DX11_DLL CDx11Object
{
protected:
	int		m_iRef;
	string	m_strTag;
	string	m_strName;
	string	m_strTypeName;
	bool	m_bEnable;
	bool	m_bAlive;

protected:
	CDx11Object();
	virtual ~CDx11Object() = 0;

public:
	string GetTag();
	string GetName();
	string GetTypeName();
	bool GetEnable();
	bool GetAlive();
	int GetRef();

public:
	void Remove();
	int Release();
	int AddRef();
	void SetTag(const string& strTag);
	void SetName(const string& strName);
	void SetEnable(bool bEnable);
	void Death();

	template <class T>
	void SetTypeName()
	{
		m_strTypeName = typeid(T).name() + 12;
	}

};

DX11_END
