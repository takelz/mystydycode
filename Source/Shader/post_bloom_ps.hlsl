struct PS_IN {
	float4 pos : SV_POSITION;
	float4 col : TEXCOORD0;
	float2 uv : TEXCOORD1;
};

Texture2D tex : register(t0);
Texture2D addTex : register(t1);

SamplerState samp : register(s0);

cbuffer ConstantBuffer :register(b0)
{
	// ‚Ç‚Ì’lˆÈã‚Ì–¾“x‚ÌF‚ğŒõ‚ç‚¹‚é‚©
	float power;
	//float2 damy;
};

float4 main(PS_IN PIN) : SV_Target
{
	float4 texColor = tex.Sample(samp, PIN.uv);	
	float4 addColor = addTex.Sample(samp, PIN.uv);

	float4 colorOut = 0;

	colorOut = texColor + (addColor * 2.0f);

	return colorOut;
}
