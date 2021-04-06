struct PS_IN {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 color : TEXCOORD1;
	float4 wpos : TEXCOORD2;
	float4 normal : TEXCOORD3;
	float   time: TEXCOORD4;
};

Texture2D tex : register(t0);
Texture2D stack : register(t1);
Texture2D rand : register(t2);
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

cbuffer ConstantBufferCamera : register(b2)
{
	float4 CameraDirection;			// 方向
	float4 CameraPos;				// 視点位置

}


float4 main(PS_IN PIN) : SV_Target
{
	float3 N = PIN.normal.xyz;
	float3 L = LightDirection.xyz;
	float4 diffuse = diffuseMaterial;
	float4 specular = float4(0.0f, 0.0f, 0.0f, 1.0f);
	//if (EyePos.w > 0.0f) {	// 光源有効?
		N = normalize(N);
		L = normalize(L);
		diffuse *= saturate(dot(L, N));
		//diffuse *= saturate(dot(L, N) * 0.5f + 0.5f);	// ハーフランバート
	
		float3 V = normalize(EyePos.xyz - PIN.wpos.xyz);
		float3 H = normalize(L + V);
	
		float s = saturate(dot(N, H));
		specular = specularMaterial * pow(s, 50);
	//}

	float4 texColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	if (textureEnable.x > 0.0f) {
		texColor = tex.Sample(samp, PIN.uv);
	}


	float4 color = diffuse * texColor * PIN.color;
	color.rgb += specular.rgb;
	

	color.a = diffuseMaterial.a * texColor.a;
	float3 vDir = normalize(CameraPos.xyz - PIN.wpos.xyz);	// カメラ向き
	float3 norm = float3(PIN.normal.x, PIN.normal.y, PIN.normal.z);



	// ベクトルが外側に行くほど薄く
	float val = 1 - abs(dot(vDir, norm));
	color.a = color.a * val * val;
	
	//+リム
	float3 rimColor = float3(0.3f, 0.3f, 1.0f);
	float rim = 1 - saturate(dot(vDir, norm));
	color.rgb += rimColor * pow(rim,2.5f);

	// emission
	float3 emiColor = float3(1.0f, 0.4f, 0.4f);
	
	// テクスチャ二枚目
	float2 uvPos = float2(PIN.uv.x, PIN.uv.y + PIN.time * 0.1f);
	float4 stackColor = stack.Sample(samp, uvPos);
	emiColor += stackColor.rgb * 0.05f;
	color.a += stackColor.r  * 0.5f;

	// ランダム
	float2 uvPos2 = float2(PIN.uv.x + PIN.time * 0.4f, PIN.uv.y + PIN.time * 0.4f);
	float4 randColor = rand.Sample(samp, uvPos2);
	color.rgb += randColor.xyz * 0.4f;
	color.a *= randColor.x;
	color.r *= 0.7f;
	color.g *= 0.7f;
	//color.a = 1.0f;

	return color;
}
