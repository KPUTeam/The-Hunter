#pragma once
#include "Dx11UI.h"

DX11_BEGIN

class DX11_DLL CDx11UISlot :
	public CDx11UI
{
private:
	friend class CDx11GameObject;

private:
	CDx11UISlot();
	CDx11UISlot(const CDx11UISlot& slot);
	~CDx11UISlot();

private:
	class CDx11Texture*		m_pOnTexture;
	function<int(CDx11UISlot*, class CDx11Collider*, float)>	m_SlotFunc;
	function<int(CDx11UISlot*, class CDx11Collider*, float)>	m_SlotDownFunc;
	bool	m_bSlotFunc;
	bool	m_bSlotDownFunc;
	bool	m_bIcon;

public:
	template <typename T>
	void SetSlotFunction(T* pObj, int (T::*pFunc)(CDx11UISlot*, class CDx11Collider*, float))
	{
		m_SlotFunc = bind(pFunc, pObj, placeholders::_1, placeholders::_2, placeholders::_3);
		m_bSlotFunc = true;
	}
	template <typename T>
	void SetSlotDownFunction(T* pObj, int (T::*pFunc)(CDx11UISlot*, class CDx11Collider*, float))
	{
		m_SlotDownFunc = bind(pFunc, pObj, placeholders::_1, placeholders::_2, placeholders::_3);
		m_bSlotDownFunc = true;
	}

	void AddIcon(class CDx11GameObject* pIcon);
	void DeleteIcon();

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11UISlot* Clone();
	virtual void OnCollisionEnter(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);
	virtual void OnCollision(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);
	virtual void OnCollisionExit(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);
};

DX11_END
