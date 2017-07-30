#pragma once
#include "AI/Dx11State.h"

DX11_USING

class CWalkState :
	public CDx11State
{
public:
	CWalkState();
	CWalkState(const CWalkState& script);
	~CWalkState();

public:
	virtual void Enter();
	virtual void Execute(float fTime);
	virtual int Exit(float fTime);
	virtual CWalkState* Clone();

private:
	Vec3 vDest;
};