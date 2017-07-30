#pragma once
#include "AI/Dx11State.h"

DX11_USING

class CDamageState :
	public CDx11State
{
public:
	CDamageState();
	CDamageState(const CDamageState& script);
	~CDamageState();

public:
	virtual void Enter();
	virtual void Execute(float fTime);
	virtual int Exit(float fTime);
	virtual CDamageState* Clone();
};