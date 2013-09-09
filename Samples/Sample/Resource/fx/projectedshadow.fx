
float4x4 matWVP;

struct VS_PROJ_OUTPUT
{
    float4 position   : POSITION;
    float2 uv0        : TEXCOORD0; 
};

VS_PROJ_OUTPUT vsProjShadow(float4 pos : POSITION,
							float2 uv0 : TEXCOORD0)
{
    VS_PROJ_OUTPUT Out;
    Out.position = mul(pos, matWVP);
    Out.uv0 = uv0;
    
    return Out;    
}

float4 psProjShadow(VS_PROJ_OUTPUT In) : COLOR
{ 
	return 0.5f;
}

technique TProjShadow
{
	pass P0
    {
        VertexShader = compile vs_1_1 vsProjShadow();
        PixelShader  = compile ps_2_0 psProjShadow();
    }
}


