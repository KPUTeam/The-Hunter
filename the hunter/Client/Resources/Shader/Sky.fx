
TextureCube		g_CubeTex	: register(t0);
SamplerState	g_LinearSmp	: register(s0)
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

cbuffer Transform	: register(b0)
{
	matrix	g_matWorld;
	matrix	g_matView;
	matrix	g_matProj;
	matrix	g_matWV;
	matrix	g_matWVP;
};

struct VS_INPUT_SKY
{
	float3		vPos	: POSITION;
};

struct VS_OUTPUT_SKY
{
	float4		vPos	: SV_POSITION;
	float3		vLocalPos	: POSITION;
};

VS_OUTPUT_SKY SkyVS(VS_INPUT_SKY input)
{
	VS_OUTPUT_SKY	output = (VS_OUTPUT_SKY)0;

	output.vPos = mul(float4(input.vPos, 1.f), g_matWVP).xyww;
	output.vLocalPos = input.vPos;

	return output;
}

struct PS_OUTPUT
{
	float4	vColor	: SV_Target;
};

PS_OUTPUT SkyPS(VS_OUTPUT_SKY input)
{
	PS_OUTPUT	output = (PS_OUTPUT)0;

	output.vColor = g_CubeTex.Sample(g_LinearSmp, input.vLocalPos);

	return output;
}