#pragma once
#include "Dx11UI.h"

DX11_BEGIN

class DX11_DLL CDx11UIButton :
	public CDx11UI
{
private:
	friend class CDx11GameObject;

public:
	CDx11UIButton();
	CDx11UIButton(const CDx11UIButton& ui);
	~CDx11UIButton();

private:
	function<int(CDx11GameObject*)>	m_Action;
	bool			m_bAction;
	BUTTON_STATE	m_eState;
	class CDx11Material*		m_pMaterial[BS_DISABLE - 1];
	Vec4			m_vColor[BS_MAX - 1];
	string			m_strBSTag[BS_MAX - 1];

public:
	template <typename T>
	bool AddAction(int (T::*pFunc)(CDx11GameObject*), T* pObj)
	{
		m_Action = bind(pFunc, pObj, placeholders::_1);
		m_bAction = true;

		return true;
	}

public:
	bool SetStateTexture(BUTTON_STATE eState, TCHAR* pFileName, const string& strKey,
		const string& strPathKey = DX11_TEXTURE_PATH);
	bool SetStateTexture(BUTTON_STATE eState, const string& strKey);
	bool SetMesh(const string& strKey, void* pVertices, UINT iVertexCount, UINT iVertexStride,
		void* pIndices, UINT iIndexCount, UINT iIndexStride, DXGI_FORMAT eIndexFmt,
		D3D11_PRIMITIVE_TOPOLOGY ePrimitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11UIButton* Clone();
	virtual void OnCollisionEnter(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);
	virtual void OnCollision(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);
	virtual void OnCollisionExit(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);
};

DX11_END
