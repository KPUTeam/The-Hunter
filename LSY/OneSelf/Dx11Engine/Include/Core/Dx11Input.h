#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

class DX11_DLL CDx11Input :
	public CDx11Object
{
	DX11_DECLARE_SINGLE(CDx11Input)

private:
	unordered_map<string, PKEYINFO>		m_mapKey;
	PKEYINFO		m_pCreateKey;

public:
	bool Init();
	int Update(float fTime);
	bool KeyDown(const string& strKey);
	bool KeyPush(const string& strKey);
	bool KeyUp(const string& strKey);
	
private:
	PKEYINFO FindKey(const string& strKey);

public:
	template <typename T>
	bool CreateKey(const T& value)
	{
		string	strType = typeid(T).name();

		if (strType == "char" || strType == "int")
		{
			m_pCreateKey->vecKey.push_back((UINT)value);
		}

		else
		{
			m_pCreateKey->strName = value;
			if (FindKey(m_pCreateKey->strName))
			{
				SAFE_DELETE(m_pCreateKey);
				return false;
			}

			m_mapKey.insert(make_pair(m_pCreateKey->strName, m_pCreateKey));
		}

		return true;
	}

	template <typename T, typename ... Types>
	bool CreateKey(const T& value, const Types&... Args)
	{
		if (!m_pCreateKey)
		{
			m_pCreateKey = new KEYINFO;

			m_pCreateKey->bDown = false;
			m_pCreateKey->bPush = false;
			m_pCreateKey->bUp = false;
		}

		string	strType = typeid(T).name();

		if (strType == "char" || strType == "int")
		{
			m_pCreateKey->vecKey.push_back((UINT)value);
		}

		else
		{
			m_pCreateKey->strName = value;
			if (FindKey(m_pCreateKey->strName))
			{
				SAFE_DELETE(m_pCreateKey);
				return false;
			}

			m_mapKey.insert(make_pair(m_pCreateKey->strName, m_pCreateKey));
		}

		if (!CreateKey(Args...))
		{
			m_pCreateKey = NULL;
			return false;
		}

		m_pCreateKey = NULL;

		return true;
	}
};

DX11_END