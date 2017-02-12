#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

class DX11_DLL CDx11Resource :
	public CDx11Object
{
protected:
	CDx11Resource();
	virtual ~CDx11Resource() =0;
};

DX11_END