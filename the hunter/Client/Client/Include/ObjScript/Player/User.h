#pragma once
#include "Component\Dx11Script.h"

DX11_USING

class CUser :
	public CDx11Script
{
public:
	CUser();
	CUser(const CUser& user);
	~CUser();

private:
	INT64		m_iSerialNumber;

public:
	INT64 GetSerialNumber() const
	{
		return m_iSerialNumber;
	}

	void SetSerialNumber(INT64 iSerialNumber)
	{
		m_iSerialNumber = iSerialNumber;
	}

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CUser* Clone();
	virtual void OnCollisionEnter(class CDx11Collider* pColl, float fTime);
	virtual void OnCollision(class CDx11Collider* pColl, float fTime);
	virtual void OnCollisionExit(class CDx11Collider* pColl, float fTime);
};

