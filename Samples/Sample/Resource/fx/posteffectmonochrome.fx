
float3 luminance = { 0.2125f, 0.7154f, 0.0721f };
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

float4 psMonoChrome(float2 uv : TEXCOORD0) : COLOR0
{
	return dot((float3)tex2D(sceneSampler, uv), luminance);
}

technique T
{
    pass P0
    {          
        VertexShader = null;
        PixelShader  = compile ps_2_0 psMonoChrome();
    }
}