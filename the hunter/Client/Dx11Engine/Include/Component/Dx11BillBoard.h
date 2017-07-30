#pragma once
#include "Dx11Component.h"

DX11_BEGIN

class DX11_DLL CDx11BillBoard :
	public CDx11Component
{
private:
	friend class CDx11GameObject;

private:
	CDx11BillBoard();
	CDx11BillBoard(const CDx11BillBoard& bill);
	~CDx11BillBoard();

public:
	Vec3	m_vecPivot;

public:
	void SetPivot(const Vec3& vecPivot);
public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11BillBoard* Clone();
};

DX11_END
