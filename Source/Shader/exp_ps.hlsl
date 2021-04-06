struct PS_IN {
	float4 pos : SV_POSITION;
	float4 wpos : TEXCOORD0;
	float4 normal : TEXCOORD1;
};

cbuffer ConstantBuffer : register(b0)
{
	float4 diffuseMaterial;

	float4 LightDirection;	// ���̕���
	float4 EyePos;			// ���_�ʒu
}

float4 main(PS_IN PIN) : SV_Target
{
	float4 N = PIN.normal;
	float4 L = LightDirection;
	N.w = 0.0f;
	L.w = 0.0f;

	N = normalize(N);
	L = normalize(L);

	float d = saturate(dot(L, N) * 0.5f + 0.5f);	// �n�[�t�����o�[�g
	float4 diffuse = diffuseMaterial * d;

	float4 V = normalize(EyePos - PIN.wpos);
	float4 H = normalize(L + V);

	float s = saturate(dot(N, H));
	float4 specular = pow(s, 50) * float4(1, 1, 1, 1);

	float4 color = specular + diffuse;
	color.a = diffuseMaterial.a;
	return color;
}
