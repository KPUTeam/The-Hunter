#pragma once
#include "Dx11Component.h"

DX11_BEGIN

class DX11_DLL CDx11UI :
	public CDx11Component
{
protected:
	CDx11UI();
	CDx11UI(const CDx11UI& ui);
	virtual ~CDx11UI();

protected:
	int		m_iZOrder;

public:
	void SetZOrder(int iZOrder);
	int GetZOrder()	const;

public:
	void SetMesh(const string& strKey);
	void SetShader(const string& strKey);
	void SetTexture(const string& strKey);
	void SetTexture(const string& strKey, TCHAR* pFileName,
		const string& strPathKey = DX11_TEXTURE_PATH);

public:
	virtual bool Init();
	virtual int Update(float fTime) = 0;
	virtual int LateUpdate(float fTime) = 0;
	virtual void Render(float fTime) = 0;
	virtual CDx11UI* Clone() = 0;
	virtual void OnCollisionEnter(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);
	virtual void OnCollision(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);
	virtual void OnCollisionExit(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);
};

DX11_END
