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
};

DX11_END
