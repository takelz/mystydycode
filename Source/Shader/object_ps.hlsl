struct PS_IN {
	float4 pos : SV_POSITION;
	float4 col : TEXCOORD0;
};

float4 main(PS_IN PIN) : SV_Target
{
	return PIN.col;
}
