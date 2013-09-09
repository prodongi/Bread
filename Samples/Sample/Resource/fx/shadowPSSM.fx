
float4x4 matVP;

void vsShadowPSSM(float4 pos : POSITION,
				  out float4 posOut : POSITION,
				  out float3 pixelOut : TEXCOORD0)
{
	posOut = mul(pos, matVP);
	pixelOut = posOut.xyz;
}

float4 psShadowPSSM(float3 pixelPos : TEXCOORD0) : COLOR
{
	return pixelPos.z;
}

technique TShadowPSSM
{
	pass p0
	{
		VertexShader = compile vs_2_0 vsShadowPSSM();
		PixelShader = compile ps_2_0 psShadowPSSM();
	}
}