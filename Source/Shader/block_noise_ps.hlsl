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
	float4 colorOut = 0;

	// ��ʃT�C�Y��16:10
	float2 uv = PIN.uv;
	uv.x = floor(uv.x * 32);
	uv.y = floor(uv.y * 20);


	// frac()�͏�������Ԃ� �v�Z���Q�l https://qiita.com/shimacpyon/items/d15dee44a0b8b3883f76
	float random = frac(sin(dot(uv, float2(12.9898, 78.233))) * 4375.5453 + time.x);

	// ��萔�l�ȏ�̓m�C�Y�Ȃ��B��������������قǌ���
	float notRand = floor(random + 0.1f);
	float rimit = min(0.3f, random);
	
	
	// �i�X�A���t�@�l��������
	float alpha = time.y;
	
	colorOut = float4(texColor.r - ((rimit * notRand) * alpha), 
		texColor.g - ((rimit * notRand) * 1.2f) * alpha, 
		texColor.b - ((rimit * notRand) * 1.2f) * alpha, 1);


	return colorOut;
}
