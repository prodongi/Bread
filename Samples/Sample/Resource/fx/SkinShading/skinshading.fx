
// lightMap ps에서 사용
float3 lightDir;

texture normalTexture;
sampler normalSampler = sampler_state
{
    Texture = <normalTexture>;
    MipFilter = NONE;
    MinFilter = LINEAR;
    MagFilter = LINEAR;

    AddressU = Wrap;
    AddressV = Wrap;
};

float PHBeckmann(float ndoth, float m)
{
	float alpha = acos(ndoth);
	float ta = tan(alpha);
	float val = 1.0f/(m*m*pow(ndoth, 4.0f)) * exp(-(ta*ta)/(m*m));
	return val;
}

float4 KSTextureCompute(float2 tex : TEXCOORD0) : COLOR0
{
	float p = abs(PHBeckmann(tex.x, 1.0f - tex.y));
	return 0.5f * pow(p, 0.1f);
}

float4 psLightMap(float2 tex : TEXCOORD0) : COLOR0
{
	float4 color;
	//float3 N = 2.0f * tex2D(normalSampler, tex).xyz - 1.0f;
	//color.xyz = max(0.0f, dot(N, lightDir));
	float3 N = tex2D(normalSampler, tex).xyz;
	color.xyz = N.xxx;
	color.a = 1.0f;
	return color;
}

technique TBeckmann
{
    pass P0
    {          
        VertexShader = NULL;
        PixelShader  = compile ps_2_0 KSTextureCompute();
    }
}

technique TLightMap
{
	pass P0
	{
		VertexShader = NULL;
		PixelShader = compile ps_2_0 psLightMap();
	}
}