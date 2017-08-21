

cbuffer Transform	: register(b0)
{
	matrix	g_matWorld;
	matrix	g_matView;
	matrix	g_matProj;
	matrix	g_matWV;
	matrix	g_matWVP;
};

cbuffer LightInfo	: register(b1)
{
	int		g_iLightType;
	float3	g_vLightEmpty;
	float4	g_vLightDif;
	float4	g_vLightAmb;
	float4	g_vLightSpc;
};

cbuffer LightDir	: register(b2)
{
	float4	g_vLightDir;
};

cbuffer	LightPoint	: register(b3)
{
	float3	g_vLightPointPos;
	float	g_fLightPointRange;
	float4	g_vLightPointAtt;
};

cbuffer Material	: register(b4)
{
	float4	g_vMtrlDif;
	float4	g_vMtrlAmb;
	float4	g_vMtrlSpc;
	float4	g_vMtrlEmv;
	float	g_fFadeAmount;
	float3	g_vMtrlEmpty;
};

cbuffer Bone	: register(b5)
{
	matrix	g_matBones[90];
	int		g_iBoneCount;
	float3	g_vBoneEmpty;
};


Texture2D		g_DiffuseTex	: register(t0);
Texture2D		g_SpecularTex	: register(t1);
Texture2D		g_NormalMapTex	: register(t2);
Texture2D		g_BoneTex		: register(t3);


SamplerState	g_DiffuseSmp	: register(s0)
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

SamplerState	g_PointSmp	: register(s1)
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = WRAP;
	AddressV = WRAP;
};

struct _tagLightResult
{
	float3	vDif;
	float3	vAmb;
	float3	vSpc;
};

_tagLightResult ComputeLight(float3 vPos, float3 vNormal, float3 vTangent, float3 vBinormal,
	float2 vUV)
{
	_tagLightResult	tResult = (_tagLightResult)0;

	float3	vLightDir = (float3)0;
	float	fAtt = 1.f;

	// 방향성
	if (g_iLightType == 1)
	{
		vLightDir = -normalize(mul(float4(g_vLightDir.xyz, 0.f), g_matView));
	}

	// 점조명
	if (g_iLightType == 2)
	{
		// 조명의 위치를 뷰공간으로 변환한다.
		float3	vLightPos = mul(float4(g_vLightPointPos, 1.f), g_matView);
		vLightDir = vLightPos - vPos;

		float	fDist = length(vLightDir);

		vLightDir = normalize(vLightDir);

		if (fDist > g_fLightPointRange)
			return tResult;

		fAtt = 1.f / dot(g_vLightPointAtt.xyz, float3(1.f, fDist, fDist * fDist));
	}

	// Spot
	if (g_iLightType == 3)
	{
	}

	if (g_vMtrlAmb.w != 0.f)
	{
		// NormalTexture에서 Normal값을 얻어온다.
		float4	vNormalMap = g_NormalMapTex.Sample(g_PointSmp, vUV);
		vNormalMap.xyz = vNormalMap.xyz * 2.f - 1.f;

		// vNormalMap은 탄젠트공간에서의 Normal 벡터이다.
		// 이 벡터를 TBN을 이용해서 뷰공간으로 변환해주어야 한다.
		float3x3	matTangentSpace =
		{
			vTangent,
			vBinormal,
			vNormal
		};

		vNormal = normalize(mul(vNormalMap.xyz, matTangentSpace));
	}

	tResult.vDif = g_vMtrlDif.xyz * g_vLightDif.xyz * dot(vNormal, vLightDir) * fAtt;
	tResult.vAmb = g_vMtrlAmb.xyz * g_vLightAmb.xyz;

	float3	vR = 2.f * dot(vLightDir, vNormal) * vNormal - vLightDir;
	vR = normalize(vR);

	float3	vView = -normalize(vPos);

	if (g_vMtrlDif.w != 0.f)
	{
		float4	vSpcTex = g_SpecularTex.Sample(g_PointSmp, vUV);
		tResult.vSpc = g_vLightSpc * vSpcTex *
			pow(max(0, dot(vR, vView)), g_vMtrlSpc.w) * fAtt;
	}

	else
	{
		tResult.vSpc = g_vLightSpc * g_vMtrlSpc *
			pow(max(0, dot(vR, vView)), g_vMtrlSpc.w) * fAtt;
	}

	return tResult;
}

struct VS_INPUT_BUMP_ANIM
{
	float3	vPos	: POSITION;
	float3	vNormal	: NORMAL;
	float2	vTex	: TEXCOORD;
	float3	vTangent	: TANGENT;
	float3	vBinormal	: BINORMAL;
	float4	vWeights	: WEIGHTS;
	float4	vIndices	: BLENDINDICES;
};

struct _tagSkinningInfo
{
	float3	vPos;
	float3	vNormal;
	float3	vTangent;
	float3	vBinormal;
};

matrix GetBoneMatrix(float idx)
{
	matrix	matBone	= 
	{
		g_BoneTex.SampleLevel(g_PointSmp, float4((idx * 4 + 0.5f) / (g_iBoneCount * 4), 0.f, 0.f, 0.f), 0),
		g_BoneTex.SampleLevel(g_PointSmp, float4((idx * 4 + 1 + 0.5f) / (g_iBoneCount * 4), 0.f, 0.f, 0.f), 0),
		g_BoneTex.SampleLevel(g_PointSmp, float4((idx * 4 + 2 + 0.5f) / (g_iBoneCount * 4), 0.f, 0.f, 0.f), 0),
		g_BoneTex.SampleLevel(g_PointSmp, float4((idx * 4 + 3 + 0.5f) / (g_iBoneCount * 4), 0.f, 0.f, 0.f), 0)
	};

	return matBone;
}

_tagSkinningInfo SkinningBump(float3 vPos, float3 vNormal,
	float3 vTangent, float3 vBinormal, float4 vWeights,
	float4 vIndices)
{
	_tagSkinningInfo	tInfo = (_tagSkinningInfo)0;

	// 가중치 배열을 만들어준다.
	float	fWeights[4];
	fWeights[0] = vWeights.x;
	fWeights[1] = vWeights.y;
	fWeights[2] = vWeights.z;
	//fWeights[3] = 1.f - fWeights[0] - fWeights[1] - fWeights[2];
	fWeights[3] = vWeights.w;

	for (int i = 0; i < 4; ++i)
	{
		matrix	matBone = (matrix)0;

		if (g_iBoneCount > 90)
		{
			matBone = GetBoneMatrix(vIndices[i]);
		}

		else
		{
			matBone = g_matBones[vIndices[i]];
		}

		tInfo.vPos += fWeights[i] * mul(float4(vPos, 1.f),
			matBone).xyz;
		tInfo.vNormal += fWeights[i] * mul(float4(vNormal, 0.f),
			matBone).xyz;
		tInfo.vTangent += fWeights[i] * mul(float4(vTangent, 0.f),
			matBone).xyz;
		tInfo.vBinormal += fWeights[i] * mul(float4(vBinormal, 0.f),
			matBone).xyz;
	}

	tInfo.vNormal = normalize(tInfo.vNormal);
	tInfo.vTangent = normalize(tInfo.vTangent);
	tInfo.vBinormal = normalize(tInfo.vBinormal);

	return tInfo;
}
