struct PS_IN {
	float4 pos : SV_POSITION;
	float4 col : TEXCOORD0;
	float2 uv : TEXCOORD1;
};

Texture2D tex : register(t0);

SamplerState samp : register(s0);

cbuffer ConstantBuffer :register(b0)
{
	// ランダム値
	float2 time;
	float2 damy;
};

// frac()は小数部を返す 計算式参考 https://qiita.com/shimacpyon/items/d15dee44a0b8b3883f76
float randNoise(float2 num) {
	return frac(sin(dot( num, float2(12.9898, 78.233))) * 4375.5453 );
};

// パ～リン用ランダム
float randPerlin(float2 num) {
	num = float2(dot(num, float2(127.1f, 311.7f)) * time.x,
					dot(num, float2(269.5f, 183.3f))); 
	return -1.0f + 2.0f * frac(sin(num)*43758.5453123f);
}

float4 main(PS_IN PIN) : SV_Target
{
	float4 texColor = tex.Sample(samp, PIN.uv);	
	float4 colorOut = 0;

	// 画面サイズは16:10
	float2 uv = PIN.uv;
	uv.x = uv.x * 1.0f * time.x * 1.6f;
	uv.y = uv.y * 1.0f * time.y;

	float2 fp = floor(uv);
	// そのブロックの左端からのオフセット値
	float2 offset = frac(uv);
	float2 under = offset * offset * (3.0f - 2.0f * offset);

	// バリューノイズ
	//float v00 = randNoise(fp + float2(0.0f, 0.0f));
	//float v10 = randNoise(fp + float2(1.0f, 0.0f));
	//float v01 = randNoise(fp + float2(0.0f, 1.0f));
	//float v11 = randNoise(fp + float2(1.0f, 1.0f));

	// パ～リンノイズ
	float v00 = randPerlin(fp + float2(0.0f, 0.0f));
	float v10 = randPerlin(fp + float2(1.0f, 0.0f));
	float v01 = randPerlin(fp + float2(0.0f, 1.0f));
	float v11 = randPerlin(fp + float2(1.0f, 1.0f));

	// なめらかな値に(バリューノイズ)
	//float v0010 = lerp(v00, v10, under.x);
	//float v0111 = lerp(v01, v11, under.x);

	// これを返すとバリューノイズ
	//colorOut = lerp(v0010, v0111, under.y);
	// 内部点からブロック外部の点へ向かうベクトルと格子点からランダムなベクトルの内積を取る
	colorOut = lerp(lerp(dot(v00, offset - float2(0, 0)), dot(v10, offset - float2(1, 0) ),under.x),
					lerp(dot(v01, offset - float2(0, 1)), dot(v11, offset - float2(1, 1) ),under.x),
					under.y) + 0.5f;

	colorOut.a = 1.0f;

	return colorOut;
}
