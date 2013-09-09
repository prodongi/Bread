
float3 lightDir;
texture baseTexture;
float4x4 matWorld;
float4x4 matView;
float4x4 matProj;
float4x4 matWVP;

// 광원의 밝기
float4 I_a = { 0.5f, 0.5f, 0.5f, 1.0f };    // 주변   조명
float4 I_b = { 1.0f, 1.0f, 0.0f, 1.0f };    // 지반구 조명
float4 I_c = { 1.0f, 0.0f, 0.0f, 1.0f };    // 천반구 조명
float4 I_d = { 1.0f, 0.9f, 0.8f, 1.0f };    // 확산   조명
float4 I_s = { 1.0f, 1.0f, 1.0f, 1.0f };    // 반사   조명

// 재질설정
float4 k_a = { 0.8f, 0.8f, 0.8f, 1.0f };    // 주변광 재질
float4 k_d = { 0.4f, 0.4f, 0.4f, 1.0f };    // 확산광 재질
float4 k_s = { 0.1f, 0.1f, 0.1f, 1.0f };    // 반사광 재질
int    n   = 32;                            // 제곱값

float3 dirFromSky = { 0.0f, -1.0f, 0.0f };

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
    float4 spec	      : COLOR1;
    float2 uv0        : TEXCOORD0; 
};

VS_OUTPUT VS(float3 pos : POSITION, 
	     float3 normal : NORMAL,
             float2 uv0 : TEXCOORD0)
{
    VS_OUTPUT Out;

    float3 L = -lightDir;
    float3 Y = -dirFromSky;
    float3 P = mul(float4(pos, 1), (float4x3)matWVP);
    float3 N = normalize(mul(normal, (float3x3)matWVP));
    float3 R = normalize(2 * dot(N, L)*N - L);
    float3 V = -normalize(P);

    float4 amb = k_a * I_a;
    float4 hemi = k_a*lerp(I_b, I_c, (dot(N,Y)+1)/2)*(1-uv0.y);
    Out.diffuse = hemi;
    Out.spec = pow(max(0, dot(R, V)), 8);
    Out.uv0 = uv0;
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
    Out.color = tex2D(baseSampler, In.uv0) * In.diffuse;// + In.spec;
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
