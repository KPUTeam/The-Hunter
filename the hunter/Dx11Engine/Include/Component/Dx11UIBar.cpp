#include "Dx11UIBar.h"
#include "Dx11Transform.h"
#include "Dx11Renderer.h"
#include "../GameObject/Dx11GameObject.h"

DX11_USING

CDx11UIBar::CDx11UIBar()
{
	SetTypeName<CDx11UIBar>();
	m_fMin = 0.f;
	m_fMax = 1.f;
	m_fValue = 0.f;
	m_fValueAcc = 0.f;
	m_fGap = 0.f;
	m_eBarDir = UBD_LEFT;
	m_eBarFlag = BF_NORMAL;
}

CDx11UIBar::CDx11UIBar(const CDx11UIBar & bar)
{
	*this = bar;
	m_iRefCount = 1;
}


CDx11UIBar::~CDx11UIBar()
{
}

bool CDx11UIBar::Init()
{
	CDx11UI::Init();

	CDx11UI::SetRenderState(DX11_RS_DEPTH_DISABLE);
	CDx11UI::SetRenderState(DX11_RS_ALPHABLEND);
	CDx11UI::SetMesh(RECT_UI_MESH);
	CDx11UI::SetShader(UI_SHADER);

	return true;
}

void CDx11UIBar::Input(float fTime)
{
}

int CDx11UIBar::Update(float fTime)
{

	return 0;
}

int CDx11UIBar::LateUpdate(float fTime)
{

	// Bar의 값 연산을 수행한다.
	float	fValue;
	float	fLength;
	
	if (m_eBarFlag != BF_SHADOW)
	{
		fValue = m_fValue - m_fMin;		// 현재 값
		fLength = m_fMax - m_fMin;		// 전체 길이
	}
	else if (m_eBarFlag == BF_SHADOW)
	{
		fValue = m_fValue - m_fMin - m_fValueAcc;
		fLength = m_fMax - m_fMin;

		if (fValue < m_fMin)
			fValue = m_fMin;
		else if (fValue > m_fMax)
			fValue = m_fMax;
	}
	
	fValue /= fLength;						// 값의 비율(%)

	switch (m_eBarDir)
	{
	case UBD_LEFT:
//		m_pTransform->SetWorldPos(m_vBarPos.x - (m_vBarSize.x * (1.f - fValue)) / 2, m_vBarPos.y, 0.f);
		m_pTransform->Scaling(m_vBarSize.x * fValue, m_vBarSize.y, 1.f);
		break;
	case UBD_RIGHT:
		m_pTransform->SetWorldPos(m_vBarPos.x + (m_vBarSize.x * (1.f - fValue)) / 2, m_vBarPos.y, 0.f);
		m_pTransform->Scaling(m_vBarSize.x * fValue, m_vBarSize.y, 1.f);
		break;
	case UBD_UP:
		m_pTransform->SetWorldPos(m_vBarPos.x, m_vBarPos.y - (m_vBarSize.y * (1.f - fValue)) / 2, 0.f);
		m_pTransform->Scaling(m_vBarSize.x, m_vBarSize.y * fValue , 1.f);
		break;
	case UBD_DOWN:
		m_pTransform->SetWorldPos(m_vBarPos.x, m_vBarPos.y + (m_vBarSize.y * (1.f - fValue)) / 2, 0.f);
		m_pTransform->Scaling(m_vBarSize.x, m_vBarSize.y * fValue, 1.f);
		break;
	}

	if (m_eBarFlag == BF_SHADOW)
	{
		if (m_fGap > 0.f)
		{
			if (m_fValueAcc < m_fGap)
			{
				if (m_fGap < 2.f)
					m_fValueAcc += fTime;
				else if (m_fGap >= 2.f)
					m_fValueAcc += fTime * m_fGap;
			}
			else
			{
				SetValue(m_fValue - m_fMin - m_fValueAcc);
				m_fValueAcc = 0.f;
				m_fGap = 0.f;
			}
		}
	}


	return 0;
}

void CDx11UIBar::Render(float fTime)
{
}

CDx11UIBar * CDx11UIBar::Clone()
{
	return new CDx11UIBar(*this);
}

float CDx11UIBar::GetValue() const
{
	return m_fValue - m_fMin;
}

Vec2 CDx11UIBar::GetBarPos() const
{
	return m_vBarPos;
}


void CDx11UIBar::SetBarDir(UIBAR_DIR eDir)
{
	m_eBarDir = eDir;
}

void CDx11UIBar::SetBarSize(const Vec2 & vSize)
{
	m_vBarSize = vSize;
	m_pTransform->Scaling(vSize.x, vSize.y, 1.f);
}

void CDx11UIBar::SetBarSize(float x, float y)
{
	m_vBarSize.x = x;
	m_vBarSize.y = y;
	m_pTransform->Scaling(m_vBarSize.x, m_vBarSize.y, 1.f);
}

void CDx11UIBar::SetBarPos(const Vec2 & vPos)
{
	m_vBarPos = vPos;
	m_pTransform->SetWorldPos(m_vBarPos.x, m_vBarPos.y, 0.f);
}

void CDx11UIBar::SetBarPos(float x, float y)
{
	m_vBarPos.x = x;
	m_vBarPos.y = y;
	m_pTransform->SetWorldPos(m_vBarPos.x, m_vBarPos.y, 0.f);
}

void CDx11UIBar::SetMinMax(float fMin, float fMax)
{
	m_fMin = fMin;
	m_fMax = fMax;
}

void CDx11UIBar::SetValue(float fValue)
{
	m_fValue = fValue;

	if (m_fValue < m_fMin)
		m_fValue = m_fMin;

	else if (m_fValue > m_fMax)
		m_fValue = m_fMax;
}

void CDx11UIBar::AddValue(float fValue)
{
	m_fValue += fValue;

	if (m_fValue < m_fMin)
		m_fValue = m_fMin;

	else if (m_fValue > m_fMax)
		m_fValue = m_fMax;
}

void CDx11UIBar::SetValueAcc(float fValue)
{
	m_fValueAcc = fValue;
}

void CDx11UIBar::SetValueGap(float fValue)
{
	m_fGap = fValue;
	m_fValueAcc = 0.f;
}

void CDx11UIBar::AddValueGap(float fValue)
{
	m_fGap += fValue;
}

void CDx11UIBar::SetBarFlag(BAR_FLAG eFlag)
{
	m_eBarFlag = eFlag;
}
