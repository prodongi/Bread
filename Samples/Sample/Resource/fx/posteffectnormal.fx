
float4x4 matWVP;
float4x4 matWV;

struct VS_OUTPUT
{
    float4 position   : POSITION;
    float2 uv0 	      : TEXCOORD0;
	float3 normal     : TEXCOORD1;
};

VS_OUTPUT vsNormal(float4 pos : POSITION, 
				   float3 normal : NORMAL,
				   float2 uv0 : TEXCOORD0)
{
    VS_OUTPUT Out;

    Out.position = mul(pos, matWVP);
	Out.normal = mul(normal, (float3x3)matWV);
	Out.uv0 = uv0;
    return Out;    
}

float4 psNormal(VS_OUTPUT In) : COLOR0
{ 
	return float4(normalize(In.normal), 1.0f);
}

technique T
{
    pass P0
    {          
        VertexShader = compile vs_1_1 vsNormal();
        PixelShader  = compile ps_2_0 psNormal();
    }
}




