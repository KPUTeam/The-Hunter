#pragma once
#include "Dx11Light.h"

DX11_BEGIN

class DX11_DLL CDx11LightPoint :
	public CDx11Light
{
private:
	friend class CDx11GameObject;

private:
	CDx11LightPoint();
	CDx11LightPoint(const CDx11LightPoint& light);
	~CDx11LightPoint();

private:
	LIGHTPOINT	m_tPoint;

public:
	void SetPointInfo(const Vec3& vPos, float fRange, const Vec3& vAtt);

public:
	virtual void SetConstant(class CDx11Shader* pShader);

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11LightPoint* Clone();
};

DX11_END
