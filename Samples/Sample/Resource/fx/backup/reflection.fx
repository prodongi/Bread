
texture baseTexture;
texture envTexture;
float4x4 matWorld;
float4x4 matView;
float4x4 matProj;
float4x4 matWVP;
float3 eyePos;
float reflectivity = 1.0f;

sampler baseSampler = sampler_state
{
    Texture = <baseTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;

    AddressU = Wrap;
    AddressV = Wrap;
};

sampler envSampler = sampler_state
{
    Texture = <envTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;

    AddressU = Wrap;
    AddressV = Wrap;
};

struct VS_OUTPUT
{
    float4 position   : POSITION;
    float2 uv0        : TEXCOORD0; 
    float3 R	      : TEXCOORD1;
};

float3 calculateReflect(float4 pos, float3 normal)
{
	float3 eyeR = normalize(eyePos - pos.xyz);
	return 2.0f * dot(eyeR, normal)*normal - eyeR;
	// return refelect(-eyeR, normal);
}

VS_OUTPUT VS(float3 pos : POSITION, 
	     float3 normal : NORMAL,
             float2 uv0 : TEXCOORD0)
{
    VS_OUTPUT Out;
    Out.position = mul(float4(pos, 1), matWVP);
    Out.uv0 = uv0;
    Out.R = calculateReflect(float4(pos, 1), normal);
    
    return Out;    
}

struct PS_OUTPUT
{
    float4 color: COLOR0;
};

PS_OUTPUT PS(VS_OUTPUT In) 
{ 
    PS_OUTPUT Out;
    float4 reflect = texCUBE(envSampler, In.R);
    float4 diffuse = tex2D(baseSampler, In.uv0);
    Out.color = lerp(diffuse, reflect, reflectivity);
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
