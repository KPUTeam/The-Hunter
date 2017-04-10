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
	int	GetZOrder() const;
	void SetZOrder(int iZOrder);

public:
	void SetMesh(const string& strKey);
	void SetShader(const string& strKey);
	void SetRenderState(const string& strKey);
	void SetRenderTypeFlag(RENDER_TYPE_FLAG eType);


public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11UI* Clone() = 0;
	virtual void OnCollisionEnter(class CDx11Collider* pColl, float fTime);
	virtual void OnCollision(class CDx11Collider* pColl, float fTime);
	virtual void OnCollisionExit(class CDx11Collider* pColl, float fTime);
};

DX11_END
