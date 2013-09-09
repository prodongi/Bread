
texture sceneTexture;
sampler sceneSampler = sampler_state
{
	Texture = <sceneTexture>;
	MinFilter = Point;
	MagFilter = Point;
	MipFilter = None;
	
	AddressU = Clamp;
	AddressV = Clamp;
};

float4 psInverse(float2 uv : TEXCOORD0) : COLOR0
{
	return 1.0f - tex2D(sceneSampler, uv);
}

technique T
{
    pass P0
    {          
        VertexShader = null;
        PixelShader  = compile ps_2_0 psInverse();
    }
}