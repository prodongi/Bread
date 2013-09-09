
// gpu gems3 에서 참조

float2 screenLightPos;
float density;
float weight;
float exposure;
float decay;

#define SAMPLE_NUM 20

texture sceneTexture;
sampler sceneSampler = sampler_state
{
    Texture = <sceneTexture>;
    MinFilter = Linear;
    MagFilter = LInear;
	MipFilter = Linear;

    AddressU = Wrap;
    AddressV = Wrap;
};

texture samplingTexture;
sampler samplingSampler = sampler_state
{
    Texture = <samplingTexture>;
    MinFilter = Linear;
    MagFilter = Linear;
	MipFilter = Linear;

    AddressU = Wrap;
    AddressV = Wrap;
};


float4 PS(float2 uv : TEXCOORD0) : COLOR
{
	float2 oriUv = uv;
	half2 deltaTexCoord = (uv - screenLightPos.xy);
	deltaTexCoord *= (1.0f / SAMPLE_NUM) * density;
	half3 color = tex2D(samplingSampler, uv);
	half illuminationDecay = 1.0f;
	for (int i = 0; i < SAMPLE_NUM; i++)  
	{  
		// Step sample location along ray.  
		uv -= deltaTexCoord;  
		// Retrieve sample at new location.  
		half3 sample = tex2D(samplingSampler, uv);  
		// Apply sample attenuation scale/decay factors.  
		sample *= illuminationDecay * weight;  
		// Accumulate combined color.  
		color += sample;  
		// Update exponential decay factor.  
		illuminationDecay *= decay;  
	} 
	
	float4 sampleColor = float4(color * exposure, 1.0);
	float4 sceneColor = tex2D(sceneSampler, oriUv);
	return sceneColor + sampleColor;
}

technique T
{
    pass P0
    {
		VertexShader = NULL;
		PixelShader  = compile ps_2_0 PS();
    }
}
	











