#pragma once
#include "Component\Dx11Script.h"

DX11_USING	

class CPlayer :
	public CDx11Script
{
public:
	CPlayer();
	~CPlayer();

private:
	float		m_fSolWSpeed;
	float		m_fSolWLimit;
	float		m_fSolWAcc;
	bool		m_bSolWEnable;

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CPlayer* Clone();

private:
	void Fire();
	void SolW(float fTime);
	void Skill2();
	void Skill3();
	void Skill4();
	class CDx11GameObject* CreateBullet(class CDx11Transform* pTransform);

private:
	bool CreateMesh();
	bool CreateTex();
};

