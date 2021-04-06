struct PS_IN {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 color : TEXCOORD1;
	float4 wpos : TEXCOORD2;
	float4 normal : TEXCOORD3;
};

Texture2D tex : register(t0);
SamplerState samp : register(s0);

cbuffer ConstantBufferMaterial : register(b0)
{
	float4	diffuseMaterial;
	float4	ambientMaterial;
	float4	specularMaterial;
	float4	textureEnable;
}

cbuffer ConstantBufferLight : register(b1)
{
	float4 LightDirection;			// Œõ‚Ì•ûŒü
	float4 EyePos;					// Ž‹“_ˆÊ’u
}

float4 main(PS_IN PIN) : SV_Target
{
	float3 normal = PIN.normal.xyz;
	normal = normalize(normal);


	return float4(normal.x,normal.y,normal.z,1.0f);
}
