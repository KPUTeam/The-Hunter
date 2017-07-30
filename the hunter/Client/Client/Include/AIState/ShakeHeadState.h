#pragma once
#include "AI/Dx11State.h"

DX11_USING

class CShakeHeadState :
	public CDx11State
{
public:
	CShakeHeadState();
	CShakeHeadState(const CShakeHeadState& script);
	~CShakeHeadState();

public:
	virtual void Enter();
	virtual void Execute(float fTime);
	virtual int Exit(float fTime);
	virtual CShakeHeadState* Clone();
};