
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

float luminance = 0.08f;
static const float middleGray = 0.18f;
static const float whiteCutoff = 0.8f;

float4 psToneMap(float2 uv : TEXCOORD0) : COLOR0
{
	float4 color;
    color = tex2D(sceneSampler, uv) * middleGray / ( luminance + 0.001f );
    color *= ( 1.0f + ( color / ( whiteCutoff * whiteCutoff ) ) );
    color /= ( 1.0f + color );
    return color;
}

technique T
{
    pass P0
    {          
        VertexShader = null;
        PixelShader  = compile ps_2_0 psToneMap();
    }
}