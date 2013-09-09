// shader x5¿¡¼­ °®°í¿È
float focusDistance;
float focusRange;
float farClip;

// original scene texture
texture oriSceneTexture;
sampler oriSceneSampler = sampler_state
{
	Texture = <oriSceneTexture>;
	MinFilter = Point;
	MagFilter = Linear;
	MipFilter = Linear;
	
	AddressU = Clamp;
	AddressV = Clamp;
};

texture depthTexture;
sampler depthSampler = sampler_state
{
	Texture = <depthTexture>;
	MinFilter = Point;
	MagFilter = Point;
	MipFilter = None;
	
	AddressU = Clamp;
	AddressV = Clamp;
};

texture blurTexture;
sampler blurSampler = sampler_state
{
	Texture = <blurTexture>;
	MinFilter = Point;
	MagFilter = Point;
	MipFilter = None;
	
	AddressU = Clamp;
	AddressV = Clamp;
};

float4 psDOF(float2 uv : TEXCOORD0) : COLOR0
{
	float4 sceneTexel = tex2D(oriSceneSampler, uv);
	float4 depthTexel = tex2D(depthSampler, uv);
	float4 blurTexel = tex2D(blurSampler, uv);
	
	float depth = depthTexel.r;		
	float sceneZ = depth * farClip;
	float blurFactor = saturate(abs(sceneZ - focusDistance)/focusRange);
			
	float4 color;
	color.rgb = lerp(sceneTexel.rgb, blurTexel.rgb, blurFactor);
	color.a = 1.0;
			
	return color;
}

technique T
{
    pass P0
    {          
        VertexShader = null;
        PixelShader  = compile ps_2_0 psDOF();
    }
}

