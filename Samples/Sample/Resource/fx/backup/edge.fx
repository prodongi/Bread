
texture baseTexture;

sampler baseSampler = sampler_state
{
    Texture = <baseTexture>;
    MipFilter = NONE;
    MinFilter = LINEAR;
    MagFilter = LINEAR;

    AddressU = Wrap;
    AddressV = Wrap;
};


struct VS_OUTPUT
{
	float4 pos:	POSITION;
	float2 tex0: TEXCOORD0;
	float2 tex1: TEXCOORD1;
	float2 tex2: TEXCOORD2;
	float2 tex3: TEXCOORD3;
};

struct PS_OUTPUT
{
    float4 color: COLOR0;
};

PS_OUTPUT PS(VS_OUTPUT In) 
{ 
	float d0 = tex2D(baseSampler, In.tex0).r - tex2D(baseSampler, In.tex1).r;
	float d1 = tex2D(baseSampler, In.tex2).r - tex2D(baseSampler, In.tex3).r;
	
	float diff = dot(d0, d0) + dot(d1, d1);
	PS_OUTPUT Out;
	Out.color = 1.0f - diff;
	
	return Out;
}

technique T0
{
    pass P0
    {
        PixelShader  = compile ps_2_0 PS();
    }
}







