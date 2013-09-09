
float3 lightDir;
float3 ambient;
texture baseTexture;
float4x4 matWorld;
float4x4 matWVP;

sampler baseSampler = sampler_state
{
    Texture = <baseTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;

    AddressU = Wrap;
    AddressV = Wrap;
};

struct VS_OUTPUT
{
    float4 position   : POSITION;
    float4 diffuse    : COLOR0;
    float2 uv0        : TEXCOORD0; 
};

VS_OUTPUT VS(float3 pos : POSITION, 
	     float3 normal : NORMAL,
             float2 uv0 : TEXCOORD0)
{
    VS_OUTPUT Out;
    Out.position = mul(float4(pos, 1), matWVP);
    float3 n = normalize(mul(normal, (float3x3)matWorld));
    Out.diffuse = max(0, dot(n, lightDir));
    Out.uv0 = uv0;
    Out.diffuse.a = 1.0f;
    Out.diffuse.xyz += ambient;
    
    return Out;    
}

struct PS_OUTPUT
{
    float4 color: COLOR0;
};

PS_OUTPUT PS(VS_OUTPUT In) 
{ 
    PS_OUTPUT Out;
    Out.color = tex2D(baseSampler, In.uv0) * In.diffuse;
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
