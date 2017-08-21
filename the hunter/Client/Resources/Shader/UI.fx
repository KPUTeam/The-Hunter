
#include "Share.fx"

Texture2D		g_MultiTex1	: register(t6);
Texture2D		g_MultiTex2	: register(t7);

// Texture VS, PS
struct VS_INPUT
{
	float3	vPos	: POSITION;
	float2	vTex	: TEXCOORD;
};

struct VS_OUTPUT
{
	float4	vPos	: SV_POSITION;
	float2	vTex	: TEXCOORD;
};

VS_OUTPUT UIVS(VS_INPUT input)
{
	VS_OUTPUT	output = (VS_OUTPUT)0;

	output.vPos = mul(float4(input.vPos, 1.f), g_matWVP);
	output.vTex = input.vTex;

	return output;
}

struct PS_OUTPUT
{
	float4	vColor	: SV_Target;
};

PS_OUTPUT UIPS(VS_OUTPUT input)
{
	PS_OUTPUT	output = (PS_OUTPUT)0;

	output.vColor = g_DiffuseTex.Sample(g_DiffuseSmp, input.vTex) * g_fFadeAmount;;

	return output;
}

PS_OUTPUT UIButtonPS(VS_OUTPUT input)
{
	PS_OUTPUT	output = (PS_OUTPUT)0;

	float4	vColorDif = g_DiffuseTex.Sample(g_DiffuseSmp, input.vTex);
	float4	vMouseOnColor = g_MultiTex1.Sample(g_DiffuseSmp, input.vTex);
	float4	vClickColor = g_MultiTex2.Sample(g_DiffuseSmp, input.vTex);

	output.vColor = vMouseOnColor * (1.f - vColorDif.a) +
		vColorDif * vColorDif.a* g_fFadeAmount;;

	return output;
}
