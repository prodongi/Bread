
float4x4 matWVP;
float edgeId;

struct VS_OUTPUT
{
    float4 position   : POSITION;
};

VS_OUTPUT VS(float3 pos : POSITION)
{
    VS_OUTPUT Out;
    Out.position = mul(float4(pos, 1), matWVP);
    return Out;    
}

struct PS_OUTPUT
{
    float4 color: COLOR0;
};

PS_OUTPUT PS(VS_OUTPUT In) 
{ 
    PS_OUTPUT Out;
    Out.color = edgeId;
    return Out;
}

technique T0
{
    pass P0
    {          
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();
    }
}






