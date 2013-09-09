
float3 intensityConverter = { 0.299f, 0.587f, 0.144f };
float4 sepiaConvert = { 0.191f, -0.054f, -0.221f, 0.0f };
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

float4 psSepia(float2 uv : TEXCOORD0) : COLOR0
{
	float Y = dot(tex2D(sceneSampler, uv), intensityConverter);
	return Y + sepiaConvert;
}

technique T
{
    pass P0
    {          
        VertexShader = null;
        PixelShader  = compile ps_2_0 psSepia();
    }
}