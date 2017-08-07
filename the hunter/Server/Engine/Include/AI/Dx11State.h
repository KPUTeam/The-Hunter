#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

class DX11_DLL CDx11State :
	public CDx11Object
{
protected:
	CDx11State();
	CDx11State(const CDx11State& com);
	virtual ~CDx11State();

private:
	friend class CDx11FSM;

protected:
	double m_dClipTime;

protected:
	class CDx11GameObject* m_pGameObject;

public:
	virtual void Enter();
	virtual void Execute(float fTime);
	virtual int Exit(float fTime);

	virtual CDx11State* Clone();
	virtual void SetGameObject(CDx11GameObject* obj);
};

DX11_END