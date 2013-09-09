
texture sceneTexture;
sampler sceneSampler = sampler_state
{
	Texture = <sceneTexture>;
	MinFilter = Point;
	MagFilter = Linear;
	MipFilter = Linear;
	
	AddressU = Clamp;
	AddressV = Clamp;
};

float4 psScaleHalf(float2 uv : TEXCOORD0) : COLOR0
{
	return tex2D(sceneSampler, uv);
}

technique T
{
    pass P0
    {          
        VertexShader = null;
        PixelShader  = compile ps_2_0 psScaleHalf();
    }
}