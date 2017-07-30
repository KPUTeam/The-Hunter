#pragma once
#include "AI/Dx11State.h"

DX11_USING

class CAttackState:
	public CDx11State
{
public:
	CAttackState();
	CAttackState(const CAttackState& script);
	~CAttackState();

public:
	virtual void Enter();
	virtual void Execute(float fTime);
	virtual int Exit(float fTime);
	virtual CAttackState* Clone();

public:
	int Attack(float fTime);
	int RemoveAttackBox(float fTime);
};