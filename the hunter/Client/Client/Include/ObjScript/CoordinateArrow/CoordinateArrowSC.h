#pragma once
#include "Component\Dx11Script.h"

DX11_USING

class CCoordinateArrowSC :
	public CDx11Script
{
public:
	CCoordinateArrowSC();
	CCoordinateArrowSC(const CCoordinateArrowSC& script);
	~CCoordinateArrowSC();

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CCoordinateArrowSC* Clone();
	void CreateCubeMesh();
};