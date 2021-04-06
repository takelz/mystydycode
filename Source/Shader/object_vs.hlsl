struct VS_IN {
	float3 pos : POSITION0;
	float4 col : COLOR0;
};
struct VS_OUT {
	float4 pos : SV_POSITION;
	float4 col : TEXCOORD0;
};

cbuffer ConstantBuffer
{
	float4x4 world;
	float4x4 viewproj;
	float4 color;
};

VS_OUT main(VS_IN VIN)
{
	VS_OUT VOUT;

	VOUT.pos.xyz = VIN.pos;
	VOUT.pos.w = 1.f;
	VOUT.pos = mul(VOUT.pos, world);
	VOUT.pos = mul(VOUT.pos, viewproj);

	VOUT.col = VIN.col * color;

	return VOUT;
}