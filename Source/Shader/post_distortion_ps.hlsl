struct PS_IN {
	float4 pos : SV_POSITION;
	float4 col : TEXCOORD0;
	float2 uv : TEXCOORD1;
};

Texture2D tex : register(t0);
// 深度バッファ
Texture2D distTex : register(t1);

SamplerState samp : register(s0);

cbuffer ConstantBuffer :register(b0)
{
	// どの値以上の明度の色を光らせるか
	float4 cameraDir;
};

float4 main(PS_IN PIN) : SV_Target
{

	// 歪ませる方向のノーマライズベクトル
	float4 distNorm = distTex.Sample(samp, PIN.uv);
	float4 colorOut = 0;
	float2 addUV;// = PIN.uv;
	//float vecDot = dot(normalize(distNorm.xy),normalize(float2(0.0f,0.0f)));;
	float3 distNum = distNorm.xyz;

	addUV.x = (distNum.x * 2 - 1.0f)*(distNum.x * 2 - 1.0f)*(distNum.x * 2 - 1.0f) * 0.009f;
	addUV.y = (distNum.y * 2 - 1.0f)*(distNum.y * 2 - 1.0f)*(distNum.y * 2 - 1.0f) * 0.009f;

	addUV = clamp(addUV, -1.0f, 1.0f);

	colorOut = tex.Sample(samp, PIN.uv - (addUV));
	
	colorOut.a = distNorm.a;

	return colorOut;
	//return float4(distNorm.x - 0.5f, distNorm.y -0.5f, distNorm.z, distNorm.a);
}
