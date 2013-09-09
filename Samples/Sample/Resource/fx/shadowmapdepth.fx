
float4x4 matWLP;		// light : world-light view-light perspective
float depthBias;

struct VS_O_DEPTH
{
	float4 pos		: POSITION;
	float2 depth 	: TEXCOORD0;
};

VS_O_DEPTH vsDepth(float4 Pos    : POSITION,
				   float3 Normal : NORMAL)
{
    VS_O_DEPTH Out = (VS_O_DEPTH)0;
	Out.pos = mul( Pos, matWLP );
    Out.depth.xy = Out.pos.zw;
    return Out;
}

float4 psDepth(VS_O_DEPTH In) : COLOR
{ 
    float depth = In.depth.x/In.depth.y;
	float bias = depth/depthBias;
	return depth + bias;
}


technique TDepth
{
    pass P0
    {
        VertexShader = compile vs_1_1 vsDepth();
        PixelShader  = compile ps_2_0 psDepth();
    }
}










