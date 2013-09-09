
float3 lightDir;
float3 eyePos;
float3 ambient;
float3 diffuse;
texture baseTexture;
texture normalTexture;
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

sampler normalSampler = sampler_state
{
    Texture = <normalTexture>;
    MipFilter = NONE;
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
    float3 L          : TEXCOORD1;	// ¹Ý»ç º¤ÅÍ
    float3 E	      : TEXCOORD2;	// ±¤¿ø º¤ÅÍ
};

VS_OUTPUT VS(float3 pos : POSITION, 
	     float3 normal : NORMAL,
	     float3 tangent : TANGENT0,
             float3 binormal : BINORMAL,
             float2 uv0 : TEXCOORD0)
{
    VS_OUTPUT Out;
    Out.position = mul(float4(pos, 1), matWVP);
    Out.uv0 = uv0;

    float3 E = eyePos - pos;
    Out.E.x = dot(E, tangent);
    Out.E.y = dot(E, binormal);
    Out.E.z = dot(E, normal);

    float3 L = -lightDir;
    Out.L.x = dot(L, tangent);
    Out.L.y = dot(L, binormal);
    Out.L.z = dot(L, normal);

    Out.diffuse = float4(diffuse, 1.0f);
    
    return Out;    
}

struct PS_OUTPUT
{
    float4 color: COLOR0;
};

PS_OUTPUT PS(VS_OUTPUT In) 
{ 
    PS_OUTPUT Out;
    float3 N = 2.0f*tex2D(normalSampler, In.uv0).xyz - 1.0f;
    float3 L = normalize(In.L);
    float3 R = reflect(-normalize(In.E), N);
    float4 baseColor = tex2D(baseSampler, In.uv0);
    
    Out.color = baseColor * In.diffuse * (max(0, dot(N, L))) 
                + float4(ambient, 0.0f)
                + 0.3f * pow(max(0, dot(R, L)), 8);
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
