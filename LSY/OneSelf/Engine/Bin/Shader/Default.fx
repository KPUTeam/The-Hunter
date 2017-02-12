
Texture2D		g_DiffuseTex	: register(t0);
SamplerState	g_DiffuseSmp	: register(s0);

cbuffer	Transform	: register(b0)
{
	matrix	g_matWorld;
	matrix	g_matView;
	matrix	g_matProj;
	matrix	g_matWV;
	matrix	g_matWVP;
};

cbuffer Material	: register(b1)
{
	float4	g_vMtrlDif;
	float4	g_vMtrlAmb;
	float4	g_vMtrlSpc;
	float4	g_vMtrlEmv;
}

cbuffer LightInfo	: register(b2)
{
	float4	g_vLightDif;
	float4	g_vLightAmb;
	float4	g_vLightSpc;
	int		g_iLightType;
	float3	g_vLightEmpty;
}

cbuffer	LightDir	: register(b3)
{
	float3	g_vLightDir;
	float	g_fLifhtDirEmpty;
}

cbuffer LightPoint	: register(b4)
{
	float3	g_vLightPos;
	float	g_fLightRange;
}

struct VS_INPUT_COLOR
{
	float3	vPos	: POSITION;
	float4	vColor	: COLOR;
};

struct VS_OUTPUT_COLOR
{
	float4	vPos	: SV_POSITION;
	float4	vColor	: COLOR;
};

VS_OUTPUT_COLOR ColorVS(VS_INPUT_COLOR input)
{
	VS_OUTPUT_COLOR	output = (VS_OUTPUT_COLOR)0;

	// xyz�� input�� ����vPos�� xyz�� �����ǰ� w�� 1.f�� �����ȴ�.
	//output.vPos = float4(input.vPos, 1.f);
	// ������ ��ġ�� ���� * �� * ������ �� ����� ���ؼ� ����� ��ȯ�ϰ�
	// ��� ��ȯ�ϰ� �������� ��ȯ���ش�.
	output.vPos = mul(float4(input.vPos, 1.f), g_matWVP);
	output.vColor = input.vColor;

	return output;
}

struct PS_OUTPUT
{
	float4	vColor	: SV_Target;
};

// �Է����� ���� SV_POSITION �Ӽ��� ���� ����ؼ� �ȵȴ�.
PS_OUTPUT ColorPS(VS_OUTPUT_COLOR input)
{
	PS_OUTPUT	output = (PS_OUTPUT)0;

	output.vColor = input.vColor;
	//output.vColor = float4(1.f, 1.f, 0.f, 1.f);

	return output;
}

// ================================== Texture ====================================
struct VS_INPUT_TEX
{
	float3	vPos	: POSITION;
	float3	vNormal	: NORMAL;
	float2	vUV		: TEXCOORD;
};

struct VS_OUTPUT_TEX
{
	float4	vPos	: SV_POSITION;
	float2	vUV		: TEXCOORD;
	float3	vViewPos	: POSITION;
	float3	vViewNormal	: NORMAL;
};

// ���� ���� ���
struct LightState
{
	float4 vDif;
	float4 vAmb;
	float4 vSpc;
};

LightState ComputeLight(float3 vViewPos, float3 vViewNormal)
{
	LightState tInfo = (LightState)0;

	float3 vLightDir = (float3)0.f;
	float  fAtt = 1.f;

	// Directional Light
	if (g_iLightType == 0)
	{
		vLightDir = -normalize(mul(float4(g_vLightDir, 0.f), g_matView).xyz);
	}

	// Point Light
	if (g_iLightType == 1)
	{
		// ������ ��ġ�� ��������� ��ȯ�Ѵ�.
		float3 vLightPos = mul(float4(g_vLightPos, 1.f), g_matView).xyz;
		vLightDir = vLightPos - vViewPos;

		float fDist = length(vLightDir);

		if (fDist > g_fLightRange)
		{
			return tInfo;
		}

		fAtt = 1.f - fDist / g_fLightRange;

		vLightDir = normalize(vLightDir);
	}

	// SpotLight
	if (g_iLightType == 2)
	{
	}

	tInfo.vDif = g_vLightDif * g_vMtrlDif * max(0, dot(vLightDir, vViewNormal)) * fAtt;
	tInfo.vAmb = g_vLightAmb * g_vMtrlAmb;

	// �ݻ纤�͸� ���Ѵ�.
	float3 vR = 2.f * dot(vLightDir, vViewNormal) * vViewNormal - vLightDir;
	vR = normalize(vR);

	float3 vView = -normalize(vViewPos);

	tInfo.vSpc = g_vLightSpc * g_vMtrlSpc * pow(max(0, dot(vView, vR)), g_vMtrlSpc.w) * fAtt;

	return tInfo;
}

VS_OUTPUT_TEX DefaultTexVS(VS_INPUT_TEX input)
{
	VS_OUTPUT_TEX	output = (VS_OUTPUT_TEX)0;

	output.vPos = mul(float4(input.vPos, 1.f), g_matWVP);
	output.vUV = input.vUV;
	output.vViewNormal = normalize(mul(float4(input.vNormal, 0.f), g_matWV));
	output.vViewPos = mul(float4(input.vPos, 1.f), g_matWV);

	return output;
}

PS_OUTPUT DefaultTexPS(VS_OUTPUT_TEX input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	LightState tLightInfo = ComputeLight(input.vViewPos, input.vViewNormal);

	clip(-1.f);

	float4 vColor = g_DiffuseTex.Sample(g_DiffuseSmp, input.vUV);
	vColor.xyz = vColor.xyz * (tLightInfo.vDif.xyz + tLightInfo.vAmb.xyz) + tLightInfo.vSpc.xyz;
	output.vColor = vColor;

	return output;
}
