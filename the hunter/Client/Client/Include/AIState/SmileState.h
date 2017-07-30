#pragma once
#include "AI/Dx11State.h"

DX11_USING

class CSmileState :
	public CDx11State
{
public:
	CSmileState();
	CSmileState(const CSmileState& script);
	~CSmileState();

public:
	virtual void Enter();
	virtual void Execute(float fTime);
	virtual int Exit(float fTime);
	virtual CSmileState* Clone();
};