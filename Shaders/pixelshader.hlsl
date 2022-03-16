// MEANS VS_OUTPUT
struct PS_INPUT
{
	float4 inPos : SV_POSITION;
	float3 inColor : COLOR;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	// MEANS COLOR
	return float4(input.inColor, 1.0f);
}