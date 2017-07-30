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
	ID3D11DeviceChild*		m_pState;
	ID3D11DeviceChild*		m_pOldState;

public:
	virtual void SetState() = 0;
	virtual void ResetState() = 0;
};

DX11_END
