#pragma once

#include "Dx11Engine.h"

DX11_BEGIN

class DX11_DLL CDx11Object
{
protected:
	CDx11Object();
	virtual ~CDx11Object() = 0;

protected:
	string	m_strTag;
	bool	m_bEnable;
	bool	m_bAlive;
	int		m_iRefCount;
	string	m_strTypeName;

public:
	string GetTag();
	string GetTypeName();
	bool GetEnable();
	bool GetAlive();

public:
	void SetTag(const string& strTag);
	void SetEnable(bool bEnable);
	void Death();
	int AddRef();
	int Release();

//	���ø��� ������ �κ��Լ������� ���⶧���� .cpp�� ���������ʰ�
//	������Ͽ� �ζ����������� �ۼ��Ѵ�.
	template <typename T>
	void SetTypeName()
	{
		m_strTypeName = typeid(T).name() + 12;
	}
// typeid.name()�Լ��� C++�� �������� ���� �������ִ� �Լ� �� �ϳ��̴�.
// �̸� Real Time Type Information(RTTI)�̶� �θ���.

};

DX11_END

