#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

class DX11_DLL CDx11AnimationClip :
	public CDx11Object
{
private:
	friend class CDx11Animation3D;

private:
	CDx11AnimationClip();
	CDx11AnimationClip(const CDx11AnimationClip& clip);
	~CDx11AnimationClip();

private:
	ANIMATIONCLIP		m_tClip;

public:
	CDx11AnimationClip* Clone()	const;
	const ANIMATIONCLIP& GetAniClip();
};

DX11_END
