
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
static const int samplingNum = 13;
float samplingV[samplingNum];
static const float blurWeights[samplingNum] = 
{
    0.002216,
    0.008764,
    0.026995,
    0.064759,
    0.120985,
    0.176033,
    0.199471,
    0.176033,
    0.120985,
    0.064759,
    0.026995,
    0.008764,
    0.002216,
};

float4 psGBlurV(float2 uv : TEXCOORD0) : COLOR0
{
	float4 color = 0;
    for (int i = 0; i < samplingNum; i++)
    {    
        color += tex2D(sceneSampler, uv + float2(0.0f, samplingV[i]) ) * blurWeights[i];
    }
    
    return color;
}

technique T
{
    pass P0
    {          
        VertexShader = null;
        PixelShader  = compile ps_2_0 psGBlurV();
    }
}