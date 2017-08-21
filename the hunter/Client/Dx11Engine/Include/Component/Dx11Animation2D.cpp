#include "Dx11Animation2D.h"
#include "../GameObject/Dx11GameObject.h"
#include "../Resources/Dx11Texture.h"
#include "../Resources/Dx11ResourcesManager.h"
#include "Dx11Renderer.h"
#include "Dx11Material.h"

DX11_USING

CDx11Animation2D::CDx11Animation2D()
{
	m_eComType = CT_ANIMATION2D;
	m_eAniOption = A2DO_LOOP;
	m_eAniType = A2D_FRAME;
	SetTypeName<CDx11Animation2D>();

	m_vecFrameTexture.reserve(100);
}

CDx11Animation2D::CDx11Animation2D(const CDx11Animation2D & anim)	:
	CDx11Component(anim)
{
	m_eAniType = anim.m_eAniType;

	m_vecFrameTexture = anim.m_vecFrameTexture;

	for (size_t i = 0; i < m_vecFrameTexture.size(); ++i)
	{
		m_vecFrameTexture[i]->AddRef();
	}
}

CDx11Animation2D::~CDx11Animation2D()
{
	Safe_Release_VecList(m_vecFrameTexture);
}

UINT CDx11Animation2D::GetFrameX() const
{
	return m_iFrameX;
}

UINT CDx11Animation2D::GetFrameY() const
{
	return m_iFrameY;
}

UINT CDx11Animation2D::GetMaxFrameX() const
{
	return m_iMaxFrameX;
}

UINT CDx11Animation2D::GetMaxFrameY() const
{
	return m_iMaxFrameY;
}

ANIMATION2D_TYPE CDx11Animation2D::GetAnimationType() const
{
	return m_eAniType;
}

void CDx11Animation2D::SetAnimation2DType(ANIMATION2D_TYPE eType)
{
	m_eAniType = eType;
}

void CDx11Animation2D::SetAnimation2DOption(ANIMATION2D_OPTION eOption)
{
	m_eAniOption = eOption;
}

void CDx11Animation2D::SetMaxFrame(UINT x, UINT y)
{
	m_iMaxFrameX = x;
	m_iMaxFrameY = y;
}

void CDx11Animation2D::SetFrame(UINT x, UINT y)
{
	m_iFrameX = x;
	m_iFrameY = y;
}

void CDx11Animation2D::SetFrameX(UINT x)
{
	m_iFrameX = x;
}

void CDx11Animation2D::SetFrameY(UINT y)
{
	m_iFrameY = y;
}

void CDx11Animation2D::SetAnimationLimitTime(float fLimitTime)
{
	m_fLimitAnimationTime = fLimitTime;
}

void CDx11Animation2D::SetAnimationStopLimitTime(float fLimitTime)
{
	m_fAnimationStopLimitTime = fLimitTime;
}

void CDx11Animation2D::AddFrameTexture(const string & strKey, TCHAR * pFileName, 
	const string & strPathKey)
{
	CDx11Texture*	pTex = DX11_GET_SINGLE(CDx11ResourcesManager)->LoadTexture(
		strKey, pFileName, strPathKey);

	if (pTex)
		m_vecFrameTexture.push_back(pTex);
}

bool CDx11Animation2D::Init()
{
	m_iFrameX = m_iFrameY = 0;
	m_iMaxFrameX = m_iMaxFrameY = 1;
	m_fAnimationTime = 0.f;
	m_fLimitAnimationTime = 1.f;
	m_fAnimationStopTime = 0.f;
	m_fAnimationStopLimitTime = 1.f;

	return true;
}

int CDx11Animation2D::Update(float fTime)
{
	m_fAnimationTime += fTime;
	m_fAnimationStopTime += fTime;

	if (m_eAniOption == A2DO_TIME)
	{
		if (m_fAnimationStopTime >= m_fAnimationStopLimitTime)
		{
			m_pGameObject->Death();
			return 0;
		}
	}

	int	iFrameMax = m_iMaxFrameX;

	if (m_eAniType == A2D_ATLAS_ALL)
		iFrameMax *= m_iMaxFrameY;

	if (m_fAnimationTime >= m_fLimitAnimationTime / iFrameMax)
	{
		m_fAnimationTime -= (m_fLimitAnimationTime / iFrameMax);

		++m_iFrameX;

		if (m_iFrameX == m_iMaxFrameX)
		{
			m_iFrameX = 0;

			if (m_eAniType == A2D_ATLAS_ALL)
			{
				++m_iFrameY;

				if (m_iFrameY == m_iMaxFrameY)
				{
					if (m_eAniOption == A2DO_LOOP || m_eAniOption == A2DO_TIME)
					{
						m_iFrameY = 0;
					}
				}
			}

			else
			{
				if (m_eAniOption == A2DO_LOOP || m_eAniOption == A2DO_TIME)
				{
					m_iFrameX = 0;
				}

				else
				{
					m_pGameObject->Death();
					return 0;
				}
			}
		}

		if (m_eAniType == A2D_FRAME)
		{
			CDx11Renderer*	pRenderer = (CDx11Renderer*)m_pGameObject->FindComponentFromType(CT_RENDERER);

			CDx11Material*	pMtrl = pRenderer->GetMaterial();

			if (m_iFrameX < m_vecFrameTexture.size())
				pMtrl->SetDiffuseTexture(m_vecFrameTexture[m_iFrameX]);

			SAFE_RELEASE(pMtrl);
			SAFE_RELEASE(pRenderer);
		}
	}

	return 0;
}

int CDx11Animation2D::LateUpdate(float fTime)
{
	return 0;
}

void CDx11Animation2D::Render(float fTime)
{
}

CDx11Animation2D * CDx11Animation2D::Clone()
{
	return nullptr;
}
