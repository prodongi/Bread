
#include "fx\\gpuani.hlsl"

float4x4 matWLP;
bool isAni;

struct VS_INPUT
{
	float4 pos 			: POSITION;
	float3 normal 		: NORMAL; 
	float4 blendWeights : BLENDWEIGHT;
	float4 blendIndices : BLENDINDICES;
};

struct VS_OUTPUT
{
    float4 pos   	: POSITION;
	float3 depth 	: TEXCOORD0;
};

VS_OUTPUT vs(VS_INPUT In)
{
	VS_OUTPUT Out;
	float4 _pos;
	float3 _normal;
	
	if (isAni)
		skinAni(_pos, _normal, In.pos, In.normal, In.blendWeights, In.blendIndices);
		
	Out.pos = mul(_pos, matWLP);
	Out.depth = Out.pos.xyz;
	return Out;	
}

float4 ps(VS_OUTPUT In) : COLOR
{
	return In.depth.z;
}

technique T
{
	pass p0
	{
		VertexShader = compile vs_2_0 vs();
		PixelShader = compile ps_2_0 ps();
	}
}