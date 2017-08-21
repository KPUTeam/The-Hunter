#pragma once
#include "AI/Dx11State.h"

DX11_USING

class CRunState:
	public CDx11State
{
public:
	CRunState();
	CRunState(const CRunState& script);
	~CRunState();

	Vec3 m_vDir = VEC3ZERO;

public:
	virtual void Enter();
	virtual void Execute(float fTime);
	virtual int Exit(float fTime);
	virtual CRunState* Clone();
};