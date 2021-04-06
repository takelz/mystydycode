struct PS_IN {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 color : TEXCOORD1;
	float4 wpos : TEXCOORD2;
	float4 normal : TEXCOORD3;
	// �e
	float4 shadowPos:TEXCOORD4;
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
	float4 LightDirection;			// ���̕���
	float4 EyePos;					// ���_�ʒu
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
	diffuse *= saturate(dot(L, N) * 0.5f + 0.5f);	// �n�[�t�����o�[�g

	float3 V = normalize(EyePos.xyz - PIN.wpos.xyz);
	float3 H = normalize(L + V);

	float s = saturate(dot(N, H));
	specular = specularMaterial * pow(s, 50);

	float4 texColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	if (textureEnable.x > 0.0f) {
		texColor = tex.Sample(samp, PIN.uv);
	}

	float4 color = diffuse * texColor * PIN.color;
	// ���C�g�F���f
	color.rgb *= lightColor;
	color.rgb += specular.rgb;
	color.a = diffuseMaterial.a * texColor.a;

	// �e����
	// �����̍��W�ɕϊ����āA�e�N�X�`����UV�Q�ƈʒu��Z�l(�[�x�l)�����ɉe������
	float2 shadowUV = PIN.shadowPos.xy / PIN.shadowPos.w;
	//return float4(shadowUV, 0, 1);
	float4 depthColor = shadow.Sample(samp, shadowUV);

	// �e�N�X�`���ɏ������܂ꂽ�[�x�l�ƍ�����`�悷��s�N�Z���̐[�x�l���r
	float depth = PIN.shadowPos.z / PIN.shadowPos.w;
	if (clamp(depth - 0.01f, 0, 1) > depthColor.r)
	{
		color.rgb = color.rgb * 0.5f;
	}

	return color;
}
