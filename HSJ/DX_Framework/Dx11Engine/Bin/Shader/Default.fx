
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
	output.vPos = float4(input.vPos, 1.f);
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

	return output;
}
