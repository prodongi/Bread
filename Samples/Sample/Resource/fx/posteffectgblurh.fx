
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
float samplingU[samplingNum];
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


float4 psGBlurH(float2 uv : TEXCOORD0) : COLOR0
{
	float4 color = 0;
    for (int i = 0; i < samplingNum; i++)
    {    
        color += tex2D(sceneSampler, uv + float2(samplingU[i], 0.0f) ) * blurWeights[i];
    }
    
    return color;
}

technique T
{
    pass P0
    {          
        VertexShader = null;
        PixelShader  = compile ps_2_0 psGBlurH();
    }
}