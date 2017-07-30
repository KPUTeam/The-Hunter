#pragma once
#include "AI/Dx11State.h"

DX11_USING

class CJumpState:
	public CDx11State
{
public:
	CJumpState();
	CJumpState(const CJumpState& script);
	~CJumpState();

public:
	virtual void Enter();
	virtual void Execute(float fTime);
	virtual int Exit(float fTime);
	virtual CJumpState* Clone();

private:
	float m_fJumpTime = 0;
	
	float m_fUp = 0;
	float m_fDown = 0;
	const float ACC = 0.05;
	
};		 