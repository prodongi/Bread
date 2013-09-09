
// gpu gems3 에서 참조
float2 screenLightPos;
float density;
float weight;
float exposure;
float decay;
texture baseTexture;
texture samplingTexture;
#define SAMPLE_NUM 20

sampler baseSampler = sampler_state
{
    Texture = <baseTexture>;
    MinFilter = POINT;
    MagFilter = POINT;

    AddressU = Wrap;
    AddressV = Wrap;
};

sampler samplingSampler = sampler_state
{
    Texture = <samplingTexture>;
    MinFilter = POINT;
    MagFilter = POINT;

    AddressU = Wrap;
    AddressV = Wrap;
};


float4 ps_sampling(float2 texCoord : TEXCOORD0) : COLOR
{
	half2 deltaTexCoord = (texCoord - screenLightPos.xy);
	deltaTexCoord *= (1.0f / SAMPLE_NUM) * density;
	half3 color = tex2D(baseSampler, texCoord);
	half illuminationDecay = 1.0f;
	for (int i = 0; i < SAMPLE_NUM; i++)  
	{  
		// Step sample location along ray.  
		texCoord -= deltaTexCoord;  
		// Retrieve sample at new location.  
		half3 sample = tex2D(baseSampler, texCoord);  
		// Apply sample attenuation scale/decay factors.  
		sample *= illuminationDecay * weight;  
		// Accumulate combined color.  
		color += sample;  
		// Update exponential decay factor.  
		illuminationDecay *= decay;  
		} 

	return float4(color * exposure, 1.0);
}

float4 ps_final(float2 texCoord : TEXCOORD0) : COLOR
{
	float4 sceneColor = tex2D(baseSampler, texCoord);
	float4 sampleColor = tex2D(samplingSampler, texCoord);
	return sceneColor + sampleColor;
}




technique TSampling
{
    pass P0
    {
		VertexShader = NULL;
		PixelShader  = compile ps_2_0 ps_sampling();
    }
}

technique TFinal
{
    pass P0
    {
		VertexShader = NULL;
        PixelShader  = compile ps_2_0 ps_final();
    }
}
	











