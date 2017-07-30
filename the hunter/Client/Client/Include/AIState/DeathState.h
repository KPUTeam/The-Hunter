#pragma once
#include "AI/Dx11State.h"

DX11_USING

class CDeathState:
	public CDx11State
{
public:
	CDeathState();
	CDeathState(const CDeathState& script);
	~CDeathState();

public:
	virtual void Enter();
	virtual void Execute(float fTime);
	virtual int Exit(float fTime);
	virtual CDeathState* Clone();
};