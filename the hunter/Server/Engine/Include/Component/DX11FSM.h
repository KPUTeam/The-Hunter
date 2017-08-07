#pragma once
#include "Dx11Component.h"
#include "../AI/Dx11State.h"
#include "../Core/Dx11Scheduler.h"

DX11_BEGIN

class DX11_DLL CDx11FSM :
	public CDx11Component
{
public:
	CDx11FSM();

	// CDx11Component*	pCom = (*iter)->Clone();
	CDx11FSM(const CDx11FSM& obj)
	{
		class CDx11State* state = obj.m_pState->Clone();
	}
	~CDx11FSM();

private:
	class CDx11State* m_pState;
	class CDx11State* m_pNextState;

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11FSM* Clone();

public:
	template <typename T>
	T*  SetState(class CDx11GameObject* obj)
	{
		SAFE_RELEASE(m_pNextState);

		T*	p = new T;

		m_pNextState = p;
		m_pNextState->SetGameObject(obj);

		m_pNextState->AddRef();

		return p;

		//SAFE_RELEASE(m_pState);

		//T*	p = new T;

		//m_pState = p;

		//m_pState->SetGameObject(obj);
		//m_pState->Enter();

		//m_pState->AddRef();
		//return p;
	}
};

DX11_END
