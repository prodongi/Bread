
uniform float4x4 matWorld;
uniform float4x4 matVP;

texture skyBox;
samplerCUBE skySampler = sampler_state
{
	Texture = <skyBox>;
	MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
	
	AddressU = MIRROR;
	AddressV = MIRROR;
	AddressW = MIRROR;
};

struct VS_INPUT
{
	float3 pos : POSITION;
};

struct VS_OUTPUT
{
	float4 pos : POSITION;
	float3 tex : TEXCOORD0;
};

struct PS_OUTPUT
{
	float4 color : COLOR0;
};

VS_OUTPUT VS(VS_INPUT In)
{
	VS_OUTPUT Out;
	
	/// In.pos는 위치값이 아니고 벡터이기 때문에 w에 0을 넣는다.
	float4 pos = float4(In.pos, 0);
	
	Out.pos = mul(pos, matWorld);
	Out.pos = mul(Out.pos, matVP).xyww;
	
	Out.tex = pos.xyz;
	
	return Out;
}

PS_OUTPUT PS(VS_OUTPUT In)
{
	PS_OUTPUT Out;
	Out.color = texCUBE(skySampler, In.tex);
	
	return Out;
}

technique T
{
    pass P0
    {          
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();
    }
}



