#include "Dx11AnimationClip.h"

DX11_USING

CDx11AnimationClip::CDx11AnimationClip()
{
}

CDx11AnimationClip::CDx11AnimationClip(const CDx11AnimationClip & clip)
{
	*this = clip;
}

CDx11AnimationClip::~CDx11AnimationClip()
{
}

CDx11AnimationClip * CDx11AnimationClip::Clone()	const
{
	return new CDx11AnimationClip(*this);
}

const ANIMATIONCLIP & CDx11AnimationClip::GetAniClip()
{
	return m_tClip;
}
