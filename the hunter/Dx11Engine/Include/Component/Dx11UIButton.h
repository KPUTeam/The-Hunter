#pragma once
#include "Dx11UI.h"

DX11_BEGIN

class DX11_DLL CDx11UIButton :
	public CDx11UI
{
private:
	friend class CDx11GameObject;

private:
	CDx11UIButton();
	CDx11UIButton(const CDx11UIButton& button);
	~CDx11UIButton();

private:
	BUTTON_STATE		m_eBtnState;
	bool				m_bMouseDown;
	function<int(CDx11UIButton* pButton, float fTime)>	m_CallBack;
	bool				m_bCallBack;
	class CDx11Texture*	m_pBtnTexture[BS_END];
	Vec4				m_vStateColor[BS_END];
	Vec2				m_vBtnSize;

public:
	void SetButtonSize(const Vec2& vSize);
	void SetStateColor(BUTTON_STATE bs, const Vec4& vColor);
	void AddButtonTexture(BUTTON_STATE bs, const string& strKey, TCHAR* pFileName = NULL,
		const string& strPathKey = TEXTURE_PATH);

	// 버튼의 기능을 클라이언트에서 정하여 함수로 제공하고,
	// 엔진은 해당 버튼 오브젝트와 그 함수를 바인드해줌.
	template<typename T>
	void SetCallBack(T* pObj, int(T::*pFunc)(CDx11UIButton*, float))
	{
		m_CallBack = bind(pFunc, pObj, placeholders::_1, placeholders::_2);
		m_bCallBack = true;
	}

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11UIButton* Clone();
	
public:	
	virtual void OnCollisionEnter(class CDx11Collider* pColl, float fTime);
	virtual void OnCollision(class CDx11Collider* pColl, float fTime);
	virtual void OnCollisionExit(class CDx11Collider* pColl, float fTime);
};

DX11_END