struct VS_IN {
	float3 pos : POSITION0;
	float4 col : COLOR0;
	float2 uv : TEXCOORD0;
};
struct VS_OUT {
	float4 pos : SV_POSITION;
	float4 col : TEXCOORD0;
	float2 uv : TEXCOORD1;
};

cbuffer ConstantBuffer
{
	float4x4 mat;
	float4 color;
};

VS_OUT main(VS_IN VIN)
{
	VS_OUT VOUT;

	VOUT.pos.xyz = VIN.pos;
	VOUT.pos.w = 1.f;
	VOUT.pos = mul(VOUT.pos, mat);

	VOUT.col = VIN.col * color;

	VOUT.uv = VIN.uv;

	return VOUT;
}