
#include "Share.fx"

cbuffer Color	: register(b6)
{
	float4	g_vColor;
}

struct VS_INPUT_COLOR
{
	float3		vPos	: POSITION;
};

struct VS_OUTPUT_COLOR
{
	float4		vPos	: SV_POSITION;
};

VS_OUTPUT_COLOR ColliderVS(VS_INPUT_COLOR input)
{
	VS_OUTPUT_COLOR	output = (VS_OUTPUT_COLOR)0;

	output.vPos = mul(float4(input.vPos, 1.f), g_matWVP);

	return output;
}

struct PS_OUTPUT
{
	float4	vColor	: SV_Target;
};

PS_OUTPUT ColliderPS(VS_OUTPUT_COLOR input)
{
	PS_OUTPUT	output = (PS_OUTPUT)0;

	output.vColor = g_vColor;
	//output.vColor = vColor * g_fFadeAmount;

	return output;
}


