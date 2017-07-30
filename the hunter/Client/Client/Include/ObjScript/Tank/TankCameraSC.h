#pragma once
#include "Component\Dx11Script.h"
#include "../../ClientKey.h"

DX11_USING

class CTankCameraSC :
	public CDx11Script
{
public:
	CTankCameraSC();
	CTankCameraSC(const CTankCameraSC& script);
	~CTankCameraSC();

private:
	const float CAMERA_INITIAL_POSX = 0.f;
	const float CAMERA_INITIAL_POSY = 2.2f;
	const float CAMERA_INITIAL_POSZ = - 6.5f;

	const float CAMERA_UPDOWN_LIMITE = 0.3;

	float CameraUpDownAmounts;
	float PreCameraUpDownAmounts;


public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CTankCameraSC* Clone();

public:
	void LimitUpDownCameraAngle(float fTime);
	void TraceCameraViewLeftRight(float fTime);
	void TraceCameraViewUpDown(float fGapY);
};