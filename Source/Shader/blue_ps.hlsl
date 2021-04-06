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
	float4 LightDirection;			// 光の方向
	float4 EyePos;					// 視点位置
}

float4 main(PS_IN PIN) : SV_Target
{
	float3 N = PIN.normal.xyz;
	float3 L = LightDirection.xyz;
	float4 diffuse = diffuseMaterial;
	float4 specular = float4(0.0f, 0.0f, 1.0f, 1.0f);
	if (EyePos.w > 0.0f) {	// 光源有効?
		N = normalize(N);
		L = normalize(L);
		//diffuse *= saturate(dot(L, N));
		diffuse *= saturate(dot(L, N) * 0.5f + 0.5f);	// ハーフランバート

		float3 V = normalize(EyePos.xyz - PIN.wpos.xyz);
		float3 H = normalize(L + V);

		float s = saturate(dot(N, H));
		specular = specularMaterial * pow(s, 50);
	}

	float4 texColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	if (textureEnable.x > 0.0f) {
		texColor = tex.Sample(samp, PIN.uv);
	}




	float4 color = diffuse * texColor * PIN.color;
	color.rgb += specular.rgb;
	color.a = diffuseMaterial.a * texColor.a;
	



	return color;
}
