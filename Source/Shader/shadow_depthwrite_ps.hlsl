struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 viewPos : TEXCOORD0;
};

Texture2D tex : register(t0);
Texture2D shadow : register(t1);
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
	float depth = PIN.viewPos.z / PIN.viewPos.w;

	return float4(depth, depth, depth, 1);
}
