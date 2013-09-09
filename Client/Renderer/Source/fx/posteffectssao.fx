
texture sceneTexture;
texture ssaoTexture;

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
	float ambient = tex2D(ssaoSampler, uv).r;
	return tex2D(sceneSampler, uv) * ambient * 0.7f;
}

technique T
{
    pass P0
    {          
        VertexShader = null;
        PixelShader  = compile ps_2_0 psScaleOri();
    }
}