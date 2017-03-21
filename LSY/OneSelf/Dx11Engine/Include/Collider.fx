
#include "Share.fx"

cbuffer Color	: register(b1)
{
	float4	g_vColor;
};

struct VS_INPUT_COLLIDER
{
	float3	vPos	: POSITION;
};

struct VS_OUTPUT_COLLIDER
{
	float4	vPos	: SV_POSITION;
};

VS_OUTPUT_COLLIDER	ColliderVS(VS_INPUT_COLLIDER input)
{
	VS_OUTPUT_COLLIDER output = (VS_OUTPUT_COLLIDER)0;

	output.vPos = mul(float4(input.vPos, 1.f), g_matWVP);

	return output;
}

struct PS_OUTPUT
{
	float4	vColor	: SV_Target;
};

PS_OUTPUT	ColliderPS(VS_OUTPUT_COLLIDER input)
{
	PS_OUTPUT	output = (PS_OUTPUT)0;

	output.vColor = g_vColor;

	return output;
}