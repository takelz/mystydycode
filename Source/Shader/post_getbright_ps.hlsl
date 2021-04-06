struct PS_IN {
	float4 pos : SV_POSITION;
	float4 col : TEXCOORD0;
	float2 uv : TEXCOORD1;
};

Texture2D tex : register(t0);
SamplerState samp : register(s0);

cbuffer ConstantBuffer :register(b0)
{
	// �ǂ̒l�ȏ�̖��x�̐F�����点�邩
	float2 power;
	float2 damy;
};

float4 main(PS_IN PIN) : SV_Target
{
	float4 texColor = tex.Sample(samp, PIN.uv);	
	float4 colorOut = 0;
	// ���邳���v�Z 
	float bright  = max(texColor.r, max(texColor.g, texColor.b));

	// ���x���p���[���傫���s�N�Z������Bloom�̑Ώۂ�
	float brooming = max(0, bright - power.x);
	brooming /= max(bright, 0.00001f);

	colorOut.rgb = brooming * texColor.rgb * 1.5f;
	colorOut.a = 1.0f;
	return colorOut;
}
