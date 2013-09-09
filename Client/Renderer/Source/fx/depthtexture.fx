
float4x4 matWVP;
float4x4 matWV;
float viewDepth;

struct VS_O_DEPTH
{
	float4 pos		: POSITION;
	float2 depth 	: TEXCOORD0;
};

VS_O_DEPTH vsDepth(float3 Pos : POSITION)
{
    VS_O_DEPTH Out = (VS_O_DEPTH)0;
	Out.pos = mul( float4(Pos, 1), matWVP );
	float3 viewpos = mul(float4(Pos, 1), matWV);
    Out.depth = viewpos.zz;
    return Out;
}

float4 psDepth(VS_O_DEPTH In) : COLOR
{ 
	float normDepth = In.depth.x/viewDepth;
	float4 color = float4(normDepth, normDepth, normDepth, normDepth);
	return color;
}


technique T
{
    pass P0
    {
        VertexShader = compile vs_1_1 vsDepth();
        PixelShader  = compile ps_2_0 psDepth();
    }
}










