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

//	템플릿을 쓸때는 부분함수적으로 쓰기때문에 .cpp에 구현하지않고
//	헤더파일에 인라인형식으로 작성한다.
	template <typename T>
	void SetTypeName()
	{
		m_strTypeName = typeid(T).name() + 12;
	}
// typeid.name()함수는 C++이 다형성을 위해 지원해주는 함수 중 하나이다.
// 이를 Real Time Type Information(RTTI)이라 부른다.

};

DX11_END

