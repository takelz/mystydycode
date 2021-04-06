struct PS_IN {
	float4 pos : SV_POSITION;
	float4 col : TEXCOORD0;
	float2 uv : TEXCOORD1;
};

Texture2D tex : register(t0);
// �[�x�o�b�t�@
Texture2D zTex : register(t1);

SamplerState samp : register(s0);

cbuffer ConstantBuffer :register(b0)
{
	// �ǂ̒l�ȏ�̖��x�̐F�����点�邩
	float power;
	float2 damy;
};

float4 main(PS_IN PIN) : SV_Target
{
	float fogDist = 0.3f;
	float4 fogColor = float4(0.75f,0.7f,0.7f, 1.0f);
	float4 texColor = tex.Sample(samp, PIN.uv);	
	float4 zBuffer = zTex.Sample(samp, PIN.uv);
	float4 colorOut = 0;

	// 0�����Ȃ̂�0�����ɂ��Ă���w�肵���������߂��Ȃ�t�H�O��0��
	float setFog = max(0.0f,(zBuffer.r) - fogDist);

	fogColor.rgb *= setFog;
	colorOut = texColor + fogColor;

	return colorOut;
}
