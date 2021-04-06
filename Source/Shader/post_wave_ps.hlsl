struct PS_IN {
	float4 pos : SV_POSITION;
	float4 col : TEXCOORD0;
	float2 uv : TEXCOORD1;
};

Texture2D tex : register(t0);
// �[�x�o�b�t�@
Texture2D noise : register(t1);

SamplerState samp : register(s0);

cbuffer ConstantBuffer :register(b0)
{
	// �ǂ̒l�ȏ�̖��x�̐F�����点�邩
	float2 time;
	float2 damy;
};

float4 main(PS_IN PIN) : SV_Target
{

	float4 texColor = tex.Sample(samp, PIN.uv);	



	return texColor;
}
