
float4x4 matWVP;
float farClip;

struct VS_INPUT
{
	float4 pos 			: POSITION;
};


struct VS_OUTPUT
{
    float4 pos   	: POSITION;
	float2 depth 	: TEXCOORD0;
};

struct PS_OUTPUT
{
    float4 color: COLOR0;
};

VS_OUTPUT VS(VS_INPUT In)
{
	VS_OUTPUT Out;
	float4 vPos = mul(In.pos, matWVP);
	vPos.z = vPos.z * vPos.w / farClip;
	Out.pos = vPos;
	Out.depth = float2(vPos.z, vPos.z);
	return Out;
}

PS_OUTPUT PS(VS_OUTPUT In) 
{ 
	PS_OUTPUT Out;
	float depth = In.depth.x;
	Out.color = float4(depth, depth, depth, 1);
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










