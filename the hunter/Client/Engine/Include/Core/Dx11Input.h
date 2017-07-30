#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

typedef struct _tagKeyInfo
{
	vector<DWORD>	vecKey;
	string	strName;
	bool	bDown;
	bool	bStay;
	bool	bUp;
}KEYINFO, *PKEYINFO;

class DX11_DLL CDx11Input :
	public CDx11Object
{
	DX11_SINGLE_DECLARE(CDx11Input)

private:
	unordered_map<string, PKEYINFO>	m_mapKey;
	PKEYINFO						m_pCreateKey;
	class CDx11GameObject*		m_pMouseObj;
	class CDx11Mouse*			m_pMouse;
	HWND							m_hWnd;
	short							m_sWheel;
	Vec2							m_vDisplayRatio;
	bool							m_bInput;

public:
	CDx11Mouse* GetMouse();
	CDx11GameObject* GetMouseObj();

public:
	bool Init(HWND hWnd);
	const PKEYINFO FindKey(const string& strName);
	void Wheel(short sWheel);
	void Update(float fTime);
	bool KeyDown(const string& strName);
	bool KeyStay(const string& strName);
	bool KeyUp(const string& strName);
	Vec2 GetMousePos()	const;
	Vec2 GetMouseGap()	const;
	void ClearInfo();
	void ResizeWindow(UINT iWidth, UINT iHeight);
	void SetInput(bool bInput);
	bool GetInput();

public:
	template <typename T>
	bool CreateKey(T data)
	{
		string	str = typeid(T).name();

		if (str == "char" || str == "int")
		{
			m_pCreateKey->vecKey.push_back((DWORD)data);
		}

		else
		{
			m_pCreateKey->strName = data;
		}

		return true;
	}

	template <typename T, typename...T1>
	bool CreateKey(const T& data, T1...args)
	{
		if (!m_pCreateKey)
		{
			m_pCreateKey = new KEYINFO;
			m_pCreateKey->bDown = false;
			m_pCreateKey->bStay = false;
			m_pCreateKey->bUp = false;
		}

		string	str = typeid(T).name();

		if (str == "char" || str == "int")
		{
			m_pCreateKey->vecKey.push_back((DWORD)data);
		}

		else
		{
			m_pCreateKey->strName = data;
		}

		CreateKey(args...);

		if (m_pCreateKey)
		{
			m_mapKey.insert(make_pair(m_pCreateKey->strName, m_pCreateKey));
			m_pCreateKey = NULL;
		}

		return true;
	}
};

DX11_END
