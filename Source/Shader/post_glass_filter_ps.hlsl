struct PS_IN {
	float4 pos : SV_POSITION;
	float4 col : TEXCOORD0;
	float2 uv : TEXCOORD1;
};

Texture2D tex : register(t0);

Texture2D rTex : register(t1);
Texture2D gTex : register(t2);
Texture2D bTex : register(t3);

SamplerState samp : register(s0);

cbuffer ConstantBuffer :register(b0)
{
	// �ǂ̒l�ȏ�̖��x�̐F�����点�邩
	float power;
	float2 damy;
};

float4 main(PS_IN PIN) : SV_Target
{

	
	float r = rTex.Sample(samp, PIN.uv).x;
	float g = gTex.Sample(samp, PIN.uv).x;
	float b = bTex.Sample(samp, PIN.uv).x;

	// -1.0f~1.0f�ɒ���
	r = r * 2 - 1;
	g = g * 2 - 1;
	b = b * 2 - 1;

	float2 uv = PIN.uv + (float2(r, g)* 0.025f);
	uv = clamp(uv, 0.05f, 0.95f);

	float4 texColor = tex.Sample(samp,uv);	

	texColor *= 0.7f;
	texColor.a = 1.0f;

	return  texColor;
}

/*
�V���h�E�}�b�v
�t�H�O
�u���[��
	�{�u���[
�K���X�t�B���^(�c��)
	�{�m�C�Y�e�N�X�`��
�o���A
�X�y�L����
�m�C�Y�G�t�F�N�g

*/