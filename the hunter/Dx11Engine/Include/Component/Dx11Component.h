#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

class DX11_DLL CDx11Component :
	public CDx11Object
{
protected:
	CDx11Component();
	CDx11Component(const CDx11Component& com);
	virtual ~CDx11Component() = 0;

protected:
//	자신의 속한 씬, 레이어, 게임오브젝트
//	그리고 자신의 컴포넌트 타입을 가지고 있다.
	class CDx11Scene*		m_pScene;
	class CDx11Layer*		m_pLayer;
	class CDx11GameObject*	m_pGameObject;
	class CDx11Transform*	m_pTransform;
	COMPONENT_TYPE			m_eComType;

public:
	void SetScene(CDx11Scene* pScene);
	void SetLayer(CDx11Layer* pLayer);
	void SetGameObject(CDx11GameObject* pGameObject);
	void SetTransform(CDx11Transform* pTransform);

public:
	CDx11GameObject* GetGameObject();
	COMPONENT_TYPE GetComponentType();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11Component* Clone() = 0;
	virtual void OnCollisionEnter(class CDx11Collider* pColl, float fTime);
	virtual void OnCollision(class CDx11Collider* pColl, float fTime);
	virtual void OnCollisionExit(class CDx11Collider* pColl, float fTime);
};

DX11_END