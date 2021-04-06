struct PS_IN {
	float4 pos : SV_POSITION;
	float4 col : TEXCOORD0;
	float2 uv : TEXCOORD1;
};

Texture2D tex : register(t0);
SamplerState samp : register(s0);

cbuffer ConstantBuffer :register(b0)
{
	float2 power;
	float2 damy;
};

float4 main(PS_IN PIN) : SV_Target
{
	//float4 texColor = tex.Sample(samp, PIN.uv);	
	//float2 otherUV = fwidth(setUV);
	float4 colorOut = 0;

	float4 setColor = 0;
	setColor += tex.Sample(samp, clamp(PIN.uv + power * float2(0.5f, 0.0f),0.0f,0.99f) );
	setColor += tex.Sample(samp, clamp(PIN.uv - power * float2(0.5f, 0.0f),0.0f,0.99f) );
	setColor += tex.Sample(samp, clamp(PIN.uv + power * float2(0.0f, 0.5f),0.0f,0.99f) );
	setColor += tex.Sample(samp, clamp(PIN.uv - power * float2(0.0f, 0.5f),0.0f,0.99f) );
	
	colorOut += setColor * 0.25f;


	return colorOut;
}
