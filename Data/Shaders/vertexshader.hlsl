cbuffer CB_VS : register(b0)
{
	float4x4 mat;
};

struct VS_INPUT
{
	float3 inPos : POSITION;
	float2 inTexCoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 outPos : SV_POSITION;
	float2 outTexCoord : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	float4 outPos = float4(input.inPos, 1.0f);
	output.outPos = mul(outPos, mat);
	output.outTexCoord = input.inTexCoord;
	return output;
}