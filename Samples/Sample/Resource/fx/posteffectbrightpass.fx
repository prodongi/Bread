
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

float Luminance = 0.08f;
static const float fMiddleGray = 0.18f;
static const float fWhiteCutoff = 0.8f;

float4 psBrightPass(float2 uv : TEXCOORD0) : COLOR0
{
	float3 color = tex2D(sceneSampler, uv);
    color *= fMiddleGray / ( Luminance + 0.001f );
    color *= ( 1.0f + ( color / ( fWhiteCutoff * fWhiteCutoff ) ) );
    color -= 5.0f;
    color = max( color, 0.0f );
    color /= ( 10.0f + color );

    return float4( color, 1.0f );
}

technique T
{
    pass P0
    {          
        VertexShader = null;
        PixelShader  = compile ps_2_0 psBrightPass();
    }
}