struct VS_IN {
	float3 pos : POSITION0;
	float3 normal : NORMAL0;
};
struct VS_OUT {
	float4 pos : SV_POSITION;
	float4 wpos : TEXCOORD0;
	float4 normal : TEXCOORD1;
};

cbuffer ConstantBuffer
{
	float4x4 world;
	float4x4 view;
	float4x4 proj;
}

VS_OUT main(VS_IN VIN)
{
	VS_OUT VOUT;

	VOUT.pos= float4(VIN.pos, 1);
	VOUT.pos = mul(VOUT.pos, world);
	VOUT.wpos = VOUT.pos;
	VOUT.pos = mul(VOUT.pos, view);
	VOUT.pos = mul(VOUT.pos, proj);

	VOUT.normal = float4(VIN.normal, 0);
	VOUT.normal = mul(VOUT.normal, world);
	VOUT.normal = normalize(VOUT.normal);

	return VOUT;
}
