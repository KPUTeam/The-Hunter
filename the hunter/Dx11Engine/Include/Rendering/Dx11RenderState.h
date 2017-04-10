#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

class DX11_DLL CDx11RenderState :
	public CDx11Object
{
protected:
	CDx11RenderState();
	virtual ~CDx11RenderState() = 0;

protected:
//	렌더 상태 3가지 모두다 DeviceChild를 상속받고 있으므로 이렇게 선언
	ID3D11DeviceChild*		m_pState;
	ID3D11DeviceChild*		m_pPrevState;
	RENDER_STATE_TYPE		m_eType;

public:
	RENDER_STATE_TYPE GetStateType();

public:
	virtual void SetState() = 0;
	virtual void ResetState() = 0;
	
};

DX11_END