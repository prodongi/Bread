
float4x4 matWorld;
float4x4 matWVP;
float3 color;

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

float4 PS(VS_OUTPUT In) : COLOR
{ 
    return float4(color.xyz, 1.0);
}

technique T0
{
    pass P0
    {          
        VertexShader = compile vs_1_1 VS();
        PixelShader  = compile ps_2_0 PS();
    }
}
