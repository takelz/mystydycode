struct PS_IN {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 color : TEXCOORD1;
	float4 wpos : TEXCOORD2;
	float4 normal : TEXCOORD3;
	// 影
	float4 shadowPos:TEXCOORD4;
};

Texture2D tex : register(t0);
Texture2D shadow : register(t1);
Texture2D addEmi : register(t2);
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

cbuffer ConstantBufferTime : register(b2)
{
	float4 time;			// xは時間、yはしきい値
}



float4 main(PS_IN PIN) : SV_Target
{
	float3 lightColor = float3(0.65f,0.55f,0.55f);
	float3 N = PIN.normal.xyz;
	float3 L = LightDirection.xyz;
	float4 diffuse = diffuseMaterial;
	float4 specular = float4(0.0f, 0.0f, 0.0f, 0.7f);

	N = normalize(N);
	L = normalize(L);
	//diffuse *= saturate(dot(L, N));
	diffuse *= saturate(dot(L, N) * 0.5f + 0.5f);	// ハーフランバート

	float3 V = normalize(EyePos.xyz - PIN.wpos.xyz);
	float3 H = normalize(L + V);

	float s = saturate(dot(N, H));
	specular = specularMaterial * pow(s, 50);
	

	float4 texColor = tex.Sample(samp, PIN.uv);
	

	float4 color = diffuse * texColor * PIN.color;
	// ライト色反映
	color.rgb *= lightColor;
	color.rgb += specular.rgb;
	color.a = diffuseMaterial.a * texColor.a;

	// ここにテクスチャ情報からemissionをランダムで追加する
	float4 disColor = addEmi.Sample(samp, PIN.uv*10);
	 
	//float emiPower = lerp(1.0, 0.0, int(disColor.r - (time.y) + 0.99f));
	if (disColor.r > time.y) {
		color.rgb += float3(-0.7f, -0.7f, -2.0f);
		//color.rgb += float3(0.8f, 0.1f, 0.1f);

	}

	// 影処理
	// 光源の座標に変換して、テクスチャのUV参照位置やZ値(深度値)を元に影をつける
	float2 shadowUV = PIN.shadowPos.xy / PIN.shadowPos.w;
	//return float4(shadowUV, 0, 1);
	float4 depthColor = shadow.Sample(samp, shadowUV);

	// テクスチャに書き込まれた深度値と今から描画するピクセルの深度値を比較
	float depth = PIN.shadowPos.z / PIN.shadowPos.w;
	if (clamp(depth - 0.01f, 0, 1) > depthColor.r)
	{
		color.rgb = color.rgb * 0.5f;
	}



	return color;
}
