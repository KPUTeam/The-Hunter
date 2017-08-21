#pragma once
#include "Dx11Collider.h"

DX11_BEGIN

class DX11_DLL CDx11ColliderTerrain :
	public CDx11Collider
{
private:
	friend class CDx11GameObject;

private:
	CDx11ColliderTerrain();
	CDx11ColliderTerrain(const CDx11ColliderTerrain& terrain);
	~CDx11ColliderTerrain();

private:
	vector<Vec3>		m_vecPos;
	vector<Vec3>		m_vecNormal;
	UINT				m_iW;
	UINT				m_iH;

public:
	void SetTerrainPos(const vector<Vec3>& vecPos, const vector<UINT>& vecIdx,
		UINT iW, UINT iH);

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11ColliderTerrain* Clone();

public:
	virtual bool Collision(CDx11Collider* pColl);
};

DX11_END
