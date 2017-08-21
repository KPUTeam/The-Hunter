#pragma once
#include "AI/Dx11State.h"

DX11_USING

class CIdleState:
	public CDx11State
{
public:
	CIdleState();
	CIdleState(const CIdleState& script);
	~CIdleState();

public:
	virtual void Enter();
	virtual void Execute(float fTime);
	virtual int Exit(float fTime);
	virtual CIdleState* Clone();
};