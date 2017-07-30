#pragma once
#include "Dx11UI.h"

DX11_BEGIN

class DX11_DLL CDx11UIIcon :
	public CDx11UI
{
private:
	friend class CDx11GameObject;

private:
	CDx11UIIcon();
	CDx11UIIcon(const CDx11UIIcon& icon);
	~CDx11UIIcon();

private:
	bool	m_bIconEnable;
	bool	m_bDrag;
	function<int(CDx11UIIcon*, float)>	m_IconFunc;
	bool	m_bIconFunc;

	function<int(CDx11UIIcon*, float)>	m_IconMouseUpFunc;
	bool	m_bIconMouseUpFunc;

public:
	void SetIconEnable(bool bEnable);
	void StopDrag();
	bool GetDrag();

	template <typename T>
	void SetIconFunction(T* pObj, int (T::*pFunc)(CDx11UIIcon*, float))
	{
		m_IconFunc = bind(pFunc, pObj, placeholders::_1, placeholders::_2);
		m_bIconFunc = true;
	}

	template <typename T>
	void SetIconMouseUpFunction(T* pObj, int (T::*pFunc)(CDx11UIIcon*, float))
	{
		m_IconMouseUpFunc = bind(pFunc, pObj, placeholders::_1, placeholders::_2);
		m_bIconMouseUpFunc = true;
	}

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11UIIcon* Clone();
	virtual void OnCollisionEnter(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);
	virtual void OnCollision(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);
	virtual void OnCollisionExit(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);
};

DX11_END
