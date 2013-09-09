
texture sceneTexture;
texture ssaoTexture;
float textureSize;

sampler sceneSampler = sampler_state
{
	Texture = <sceneTexture>;
	MinFilter = Point;
	MagFilter = Point;
	MipFilter = None;
	
	AddressU = Clamp;
	AddressV = Clamp;
};

sampler ssaoSampler = sampler_state
{
	Texture = <ssaoTexture>;
	MinFilter = Point;
	MagFilter = Point;
	MipFilter = None;
	
	AddressU = Clamp;
	AddressV = Clamp;
};

float4 psScaleOri(float2 uv : TEXCOORD0) : COLOR0
{
	// blur
	float s = 1.0f/textureSize;
	float ambient = 0.0f;
	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{
			float2 offset = float2(s*float(x), s*float(y));
			ambient += tex2D(ssaoSampler, uv + offset);
		}
	}
	ambient *= 0.0625f;
	
	return tex2D(sceneSampler, uv) * ambient;
}

technique T
{
    pass P0
    {          
        VertexShader = null;
        PixelShader  = compile ps_2_0 psScaleOri();
    }
}