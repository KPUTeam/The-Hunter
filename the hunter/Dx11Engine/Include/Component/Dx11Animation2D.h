#pragma once
#include "Dx11Component.h"

DX11_BEGIN

class DX11_DLL CDx11Animation2D :
	public CDx11Component
{
public:
	CDx11Animation2D();
	CDx11Animation2D(const CDx11Animation2D& anim);
	~CDx11Animation2D();

private:
	ANIMATION2D_OPTION			m_eAniOption;
	ANIMATION2D_TYPE			m_eAniType;
	UINT						m_iFrameX;
	UINT						m_iFrameY;
	UINT						m_iMaxFrameX;
	UINT						m_iMaxFrameY;
	float						m_fAnimationTime;
	float						m_fLimitAnimationTime;
	float						m_fAnimationStopTime;
	float						m_fAnimationStopLimitTime;
	vector<class CDx11Texture*>	m_vecFrameTexture;

public:
	UINT	GetFrameX() const;
	UINT	GetFrameY() const;
	UINT	GetMaxFrameX() const;
	UINT	GetMaxFrameY() const;
	ANIMATION2D_TYPE GetAnimationType() const;

public:
	void SetAnimation2DType(ANIMATION2D_TYPE eType);
	void SetAnimation2DOption(ANIMATION2D_OPTION eOption);
	void SetMaxFrame(UINT x, UINT y);
	void SetFrame(UINT x, UINT y);
	void SetFrameX(UINT x);
	void SetFrameY(UINT y);
	void SetAnimationLimitTime(float fLimitTime);
	void SetAnimationStopLimitTime(float fLimitTime);
	void AddFrameTexture(const string& strKey, TCHAR* pFileName,
		const string& strPathKey = TEXTURE_PATH);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11Animation2D* Clone();
};

DX11_END
