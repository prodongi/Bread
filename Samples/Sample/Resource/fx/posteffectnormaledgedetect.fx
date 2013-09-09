
texture normalTexture;
sampler normalSampler = sampler_state
{
	Texture = <normalTexture>;
	MinFilter = Point;
	MagFilter = Point;
	MipFilter = None;
	
	AddressU = Clamp;
	AddressV = Clamp;
};

float2 texelSampling[4];

float4 psNormalEdgeDetect(float2 uv : TEXCOORD0) : COLOR0
{
	float4 color = tex2D(normalSampler, uv);
    float4 sum = 0;
    for (int i = 0; i < 4; i++)
        sum += saturate( 1.0f - dot(color.xyz, tex2D(normalSampler, uv + texelSampling[i]).xyz ));

    return sum;
}

technique T
{
    pass P0
    {          
        VertexShader = null;
        PixelShader  = compile ps_2_0 psNormalEdgeDetect();
    }
}