#include "Dx11Animation3D.h"
#include "Dx11AnimationClip.h"
#include "../Shader/Dx11Shader.h"
#include "Dx11Renderer.h"
#include "../GameObject/Dx11GameObject.h"
#include "../Resources/Dx11Texture.h"
#include "../Resources/Dx11ResourcesManager.h"
#include "../Core/Dx11Debug.h"

DX11_USING

CDx11Animation3D::CDx11Animation3D()
{
	m_eComType = CT_ANIMATION3D;
	SetTypeName<CDx11Animation3D>();

	m_iCurrentClip = 0;
	m_pBoneTexture = NULL;
}

CDx11Animation3D::CDx11Animation3D(const CDx11Animation3D & anim)	:
	CDx11Component(anim)
{
	m_iCurrentClip = anim.m_iCurrentClip;
	m_fGlobalTime = anim.m_fGlobalTime;
	m_vecBoneHierarchy = anim.m_vecBoneHierarchy;
	m_tBoneCBuffer = anim.m_tBoneCBuffer;
	m_tAllClip = anim.m_tAllClip;

	m_vecAnimationClip.clear();

	m_vecAnimationClip.reserve(anim.m_vecAnimationClip.size());

	for (size_t i = 0; i < anim.m_vecAnimationClip.size(); ++i)
	{
		CDx11AnimationClip*	pClip = anim.m_vecAnimationClip[i]->Clone();

		m_vecAnimationClip.push_back(pClip);
	}

	m_pBoneTexture = NULL;

	if (m_vecBoneHierarchy.size() > 90)
	{
		m_pBoneTexture = new CDx11Texture;

		m_pBoneTexture->CreateTexture(m_vecBoneHierarchy.size() * 4, 1, 1,
			DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_USAGE_DYNAMIC,
			D3D11_BIND_SHADER_RESOURCE,
			D3D11_CPU_ACCESS_WRITE);
	}

	/*m_pBoneTexture = anim.m_pBoneTexture;

	if(m_pBoneTexture)
		m_pBoneTexture->AddRef();*/
}

CDx11Animation3D::~CDx11Animation3D()
{
	SAFE_RELEASE(m_pBoneTexture);
	Safe_Release_VecList(m_vecAnimationClip);
}

CDx11AnimationClip * CDx11Animation3D::GetAnimationClip(int idx)
{
	if (m_vecAnimationClip.empty())
		return NULL;

	return m_vecAnimationClip[idx];
}

bool CDx11Animation3D::Init()
{
	m_fGlobalTime = 0.f;

	return true;
}

int CDx11Animation3D::Update(float fTime)
{
	m_fGlobalTime += fTime;

	if (m_vecAnimationClip.empty())
	{
		if (m_fGlobalTime >= m_tAllClip.dEndTime ||
			m_fGlobalTime < m_tAllClip.dStartTime)
			m_fGlobalTime = m_tAllClip.dStartTime;
	}

	else
	{
		if (m_fGlobalTime >= m_vecAnimationClip[m_iCurrentClip]->m_tClip.dEndTime ||
			m_fGlobalTime < m_vecAnimationClip[m_iCurrentClip]->m_tClip.dStartTime)
			m_fGlobalTime = m_vecAnimationClip[m_iCurrentClip]->m_tClip.dStartTime;
	}

	vector<BONE>::iterator	iterB;
	vector<BONE>::iterator	iterBEnd = m_vecBoneHierarchy.end();
	int		iBoneIndex = 0;

	for (iterB = m_vecBoneHierarchy.begin(); iterB != iterBEnd; ++iterB)
	{
		(*iterB).matBone = XMMatrixIdentity();
		(*iterB).matTransform = XMMatrixIdentity();
		if ((*iterB).vecKeyFrame.empty())
			continue;

		vector<ANIMATIONKEYFRAME>::iterator	iterA;
		vector<ANIMATIONKEYFRAME>::iterator	iterAEnd = (*iterB).vecKeyFrame.end();
		--iterAEnd;
		for (iterA = (*iterB).vecKeyFrame.begin(); iterA != iterAEnd; ++iterA)
		{
			vector<ANIMATIONKEYFRAME>::iterator	iterANext = iterA + 1;
			if (m_fGlobalTime >= (*iterA).fAnimationTime &&
				m_fGlobalTime <= (*iterANext).fAnimationTime)
			{
				float	fPercent = (m_fGlobalTime - (*iterA).fAnimationTime) /
					((*iterANext).fAnimationTime - (*iterA).fAnimationTime);

				Vector	vS1 = (*iterA).vScale.Convert();
				Vector	vS2 = (*iterANext).vScale.Convert();

				Vector	vP1 = (*iterA).vTranslate.Convert();
				Vector	vP2 = (*iterANext).vTranslate.Convert();

				Vector	vR1 = (*iterA).qRot.Convert();
				Vector	vR2 = (*iterANext).qRot.Convert();

				// 보간한다.
				Vector	vS = XMVectorLerp(vS1, vS2, fPercent);
				Vector	vP = XMVectorLerp(vP1, vP2, fPercent);
				Vector	vR = XMQuaternionSlerp(vR1, vR2, fPercent);

				Vector	vZero = XMVectorSet(0.f, 0.f, 0.f, 1.f);
				(*iterB).matTransform = XMMatrixAffineTransformation(vS, vZero, vR, vP);
				break;
			}
		}
	}

	// 각 본에 부모 행렬을 곱해둔다.
	/*vector<Matrix>	vecParent(m_vecBoneHierarchy.size());
	vecParent[0] = m_vecBoneHierarchy[0].matTransform;

	for (size_t i = 1; i < m_vecBoneHierarchy.size(); ++i)
	{
		Matrix	matBone = m_vecBoneHierarchy[i].matTransform;

		int	iParent = m_vecBoneHierarchy[i].iParentIndex;
		Matrix	matParent = vecParent[iParent];

		vecParent[i] = matBone * matParent;
	}*/

	iBoneIndex = 0;
	vector<Matrix>	vecBones;
	vecBones.reserve(m_vecBoneHierarchy.size());

	for (iterB = m_vecBoneHierarchy.begin(); iterB != iterBEnd; ++iterB, ++iBoneIndex)
	{
		// 본 행렬을 구해주었으면 본 행렬에 offset 행렬을 곱해준다.
		// 최종 본 행렬을 만들어준다.
		(*iterB).matBone = (*iterB).matOffset * (*iterB).matTransform;
		//(*iterB).matBone = (*iterB).matOffset * vecParent[iBoneIndex];
		if (!m_pBoneTexture)
			m_tBoneCBuffer.pBones[iBoneIndex] = XMMatrixTranspose((*iterB).matBone);

		else
			vecBones.push_back((*iterB).matBone);
		//m_tBoneCBuffer.matBones[iBoneIndex] = (*iterB).matBone;
	}
	
	CDx11Renderer*	pRenderer = (CDx11Renderer*)m_pGameObject->FindComponentFromType(CT_RENDERER);

	if (pRenderer)
	{
		pRenderer->UpdateCBuffer("BoneArray", &m_tBoneCBuffer, sizeof(BONECBUFFER));
	}

	// 렌더러를 갱신한다.
	if (m_pBoneTexture)
	{
		m_pBoneTexture->SetVTFEnable();
		m_pBoneTexture->SetData(&vecBones[0], sizeof(Matrix) * m_vecBoneHierarchy.size());
		pRenderer->SetBoneTexture(m_pBoneTexture);
	}
	SAFE_RELEASE(pRenderer);

	return 0;
}

int CDx11Animation3D::LateUpdate(float fTime)
{
	return 0;
}

void CDx11Animation3D::Render(float fTime)
{
}

CDx11Animation3D * CDx11Animation3D::Clone()
{
	return new CDx11Animation3D(*this);
}

void CDx11Animation3D::SetMeshName(const string & strName)
{
	m_strName = strName;
}

void CDx11Animation3D::CreateBones(vector<BONE>& vecBone)
{
	m_vecBoneHierarchy = vecBone;
	m_tBoneCBuffer.iBoneCount = vecBone.size();

	if (vecBone.size() > 90)
	{
		string	strBoneName = m_strName + "Bone";

		SAFE_RELEASE(m_pBoneTexture);
		m_pBoneTexture = new CDx11Texture;

		m_pBoneTexture->CreateTexture(vecBone.size() * 4, 1, 1,
			DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_USAGE_DYNAMIC,
			D3D11_BIND_SHADER_RESOURCE,
			D3D11_CPU_ACCESS_WRITE);
	}
}

void CDx11Animation3D::AddAnimationClip(const ANIMATIONCLIP & tClip)
{
	CDx11AnimationClip*	pAnimationClip = new CDx11AnimationClip;

	pAnimationClip->m_tClip = tClip;

	m_vecAnimationClip.push_back(pAnimationClip);
}


void CDx11Animation3D::SetAllClip(const ANIMATIONCLIP & tClip)
{
	m_tAllClip = tClip;
}


void CDx11Animation3D::UpdateConstantBuffer(CDx11Shader * pShader)
{
	pShader->UpdateConstantBuffer("BoneArray", &m_tBoneCBuffer, ST_VERTEX);
	if(m_pBoneTexture)
		m_pBoneTexture->SetTexture(3, 1);
}

void CDx11Animation3D::ChangeAnimation(int iClip)
{
	if (m_iCurrentClip == iClip)
		return;

	m_iCurrentClip = iClip;
}

void CDx11Animation3D::ChangeAnimation(char* strName)
{
	int cnt = 0;;

	vector<CDx11AnimationClip*>::iterator iter;
	vector<CDx11AnimationClip*>::iterator iterEnd = m_vecAnimationClip.end();

	for(iter = m_vecAnimationClip.begin(); iter != iterEnd; ++iter)
	{ 
		if ((*iter)->m_tClip.strName == strName)
		{
			m_iCurrentClip = cnt;
			m_fGlobalTime = 0;
		}

		++cnt;
	}
}

double CDx11Animation3D::GetCilpTime(char * strName)
{
	int cnt = 0;;

	vector<CDx11AnimationClip*>::iterator iter;
	vector<CDx11AnimationClip*>::iterator iterEnd = m_vecAnimationClip.end();

	for (iter = m_vecAnimationClip.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->m_tClip.strName == strName)
		{
			return (m_vecAnimationClip[cnt]->m_tClip.dEndTime - m_vecAnimationClip[cnt]->m_tClip.dStartTime);
		}

		++cnt;
	}

	return 0;
}


void CDx11Animation3D::CreateClip(char * strName, double dStartTime, double dEndTime)
{
	CDx11AnimationClip*	pAnimationClip = new CDx11AnimationClip;

	pAnimationClip->m_tClip.strName = strName;
	pAnimationClip->m_tClip.dStartTime = dStartTime;
	pAnimationClip->m_tClip.dEndTime = dEndTime;

	m_vecAnimationClip.push_back(pAnimationClip);

	// SAFE_RELEASE(pAnimationClip);
}
