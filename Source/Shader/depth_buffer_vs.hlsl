struct VS_IN {
	float3 pos : POSITION0;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
	float4 weight : BLENDWEIGHT0;
	uint4 index : BLENDINDICES0;
};
struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 viewPos : TEXCOORD2;
};


cbuffer ConstantBuffer : register(b0)
{
	float4x4 world;
	float4x4 view;
	float4x4 proj;
	float4x4 Bone[120];
	float4 time;
	// ‰e
	float4x4 shadowViewProj;
}



VS_OUT main(VS_IN VIN)
{
	VS_OUT VOUT;

	VOUT.pos.xyz = VIN.pos;
	VOUT.pos.w = 1.f;

	float4x4 mat = 0;
	mat += Bone[VIN.index.x] * VIN.weight.x;
	mat += Bone[VIN.index.y] * VIN.weight.y;
	mat += Bone[VIN.index.z] * VIN.weight.z;
	mat += Bone[VIN.index.w] * VIN.weight.w;

	VOUT.pos = mul(VOUT.pos, mat);
	VOUT.pos = mul(VOUT.pos, world);
	VOUT.pos = mul(VOUT.pos, view);
	VOUT.pos = mul(VOUT.pos, proj);


	// ŒõŒ¹‚©‚çŒ©‚½ˆÊ’u
	VOUT.viewPos = VOUT.pos;

	return VOUT;
}