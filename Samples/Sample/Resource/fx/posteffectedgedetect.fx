
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

float2 texelSampling[4];

float3 luminanceConv = { 0.2125f, 0.7154f, 0.0721f };

float4 psEdgeDetect(float2 uv : TEXCOORD0) : COLOR0
{
	float4 color = tex2D(sceneSampler, uv);
    float4 sum = 0;
    for(int i = 0; i < 4; i++)
        sum += ( abs(color - tex2D(sceneSampler, uv + texelSampling[i])) - 0.5f) * 1.2f + 0.5f;

    return saturate(dot(luminanceConv, sum)) * 5.0f;
}

technique T
{
    pass P0
    {          
        VertexShader = null;
        PixelShader  = compile ps_2_0 psEdgeDetect();
    }
}