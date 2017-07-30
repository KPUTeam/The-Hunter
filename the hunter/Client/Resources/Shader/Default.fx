
#include "Share.fx"

struct VS_INPUT_COLOR
{
	float3		vPos	: POSITION;
	float4		vColor	: COLOR;
};

struct VS_OUTPUT_COLOR
{
	float4		vPos	: SV_POSITION;
	float4		vColor	: COLOR;
};

VS_OUTPUT_COLOR DefaultColorVS(VS_INPUT_COLOR input)
{
	VS_OUTPUT_COLOR	output = (VS_OUTPUT_COLOR)0;

	//output.vPos = float4(input.vPos, 1.f);
	output.vPos = mul(float4(input.vPos, 1.f), g_matWVP);
	output.vColor = input.vColor;

	return output;
}

struct PS_OUTPUT
{
	float4	vColor	: SV_Target;
};

PS_OUTPUT DefaultColorPS(VS_OUTPUT_COLOR input)
{
	PS_OUTPUT	output = (PS_OUTPUT)0;

	// output.vColor = input.vColor;
	float4 vColor = input.vColor;
	output.vColor = vColor * g_fFadeAmount;

	return output;
}

// Texture VS, PS
struct VS_INPUT_TEX
{
	float3	vPos	: POSITION;
	float3	vNormal	: NORMAL;
	float2	vTex	: TEXCOORD;
	float3	vTangent	: TANGENT;
	float3	vBinormal	: BINORMAL;
};

struct VS_OUTPUT_TEX
{
	float4	vPos	: SV_POSITION;
	float3	vNormal	: NORMAL;
	float2	vTex	: TEXCOORD;
	float3	vViewPos	: POSITION;
	float3	vTangent	: TANGENT;
	float3	vBinormal	: BINORMAL;
};

VS_OUTPUT_TEX DefaultTexVS(VS_INPUT_TEX input)
{
	VS_OUTPUT_TEX	output = (VS_OUTPUT_TEX)0;

	output.vPos = mul(float4(input.vPos, 1.f), g_matWVP);
	output.vTex = input.vTex;

	// 조명처리를 위한 정보
	output.vNormal = normalize(mul(float4(input.vNormal, 0.f), g_matWV));
	output.vViewPos = mul(float4(input.vPos, 1.f), g_matWV);

	if (g_vMtrlAmb.w != 0.f)
	{
		output.vTangent = normalize(mul(float4(input.vTangent, 0.f), g_matWV));
		output.vBinormal = normalize(mul(float4(input.vBinormal, 0.f), g_matWV));
	}

	return output;
}

// Animation 용 VS
VS_OUTPUT_TEX DefaultAnimationTexVS(VS_INPUT_BUMP_ANIM input)
{
	VS_OUTPUT_TEX	output = (VS_OUTPUT_TEX)0;

	_tagSkinningInfo	tSkinning = (_tagSkinningInfo)0;

	tSkinning = SkinningBump(input.vPos, input.vNormal,
		input.vTangent, input.vBinormal, input.vWeights,
		input.vIndices);

	output.vPos = mul(float4(tSkinning.vPos, 1.f), g_matWVP);
	output.vTex = input.vTex;

	// 조명처리를 위한 정보
	output.vNormal = normalize(mul(float4(tSkinning.vNormal, 0.f), g_matWV));
	output.vViewPos = mul(float4(tSkinning.vPos, 1.f), g_matWV);

	if (g_vMtrlAmb.w != 0.f)
	{
		output.vTangent = normalize(mul(float4(tSkinning.vTangent, 0.f), g_matWV));
		output.vBinormal = normalize(mul(float4(tSkinning.vBinormal, 0.f), g_matWV));
	}

	return output;
}

PS_OUTPUT DefaultTexPS(VS_OUTPUT_TEX input)
{
	PS_OUTPUT	output = (PS_OUTPUT)0;

	_tagLightResult	tLight	= ComputeLight(input.vViewPos, input.vNormal,
		input.vTangent, input.vBinormal, input.vTex);

	float4	vColor = g_DiffuseTex.Sample(g_DiffuseSmp, input.vTex);
	output.vColor.xyz = vColor.xyz * (tLight.vDif + tLight.vAmb) + tLight.vSpc + g_vMtrlEmv.xyz;
	output.vColor.a = vColor.a;

	output.vColor = vColor * g_fFadeAmount;

	return output;
}
