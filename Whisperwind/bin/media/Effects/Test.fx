
sampler TestSampler = 
sampler_state
{
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

void VS(float4 pos : POSITION,
              out float4 oPos : POSITION)
{
	oPos = pos;
}

void PS(out float4 oColor : COLOR0)
{
	oColor = float4(1.0f, 0.0f, 0.0f, 1.0f);
}

technique Test
{
	pass p0
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS();
	}
}