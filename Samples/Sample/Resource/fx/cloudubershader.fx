
#include "fx\ubershared.hlsl"
#include "fx\ubersampler.hlsl"

float4x4 matWVP;
float3 lightPos;
float blend;
float noiseScale1;
float noiseScale2;
float implictBlend;

struct VS_INPUT
{
	float4 pos : POSITION;
	float4 color : COLOR0;
	float4 uv : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 pos : POSITION;
	float4 color : COLOR0;
	float4 uv : TEXCOORD0;
};

struct PS_OUTPUT
{
	float4 color : COLOR0;
};

VS_OUTPUT VS(VS_INPUT In)
{
	VS_OUTPUT Out;
	Out.pos = mul(In.pos, matWVP);
	Out.color = In.color;
	Out.uv = In.uv;
	
	return Out;
}

PS_OUTPUT PS(VS_OUTPUT In)
{
	PS_OUTPUT Out;
	
	float4 point1 = In.uv;
	float4 point2 = In.uv;
	float4 perturb = tex3D(diffuseSampler, In.uv.xyz) + 
					 tex3D(diffuseSampler, 2*In.uv.xyz)*0.5f + 
					 tex3D(diffuseSampler, 4*In.uv.xyz)*0.25f + 
					 tex3D(diffuseSampler, 8*In.uv.xyz)*0.125f;
	float4 turbperturb = 2*(tex3D(diffuseSampler, In.uv.xyz) - 0.5f);
	float3 normal = perturb.xyz/sqrt(dot(perturb.xyz, perturb.xyz));
	point1.xyz += (perturb.www/noiseScale1);
	point2.xyz += (perturb.www/noiseScale2);
	float d = sqrt((point1.z-0.5f)*(point1.z-0.5f)+(point1.y-0.5f)*(point1.y-0.5f)+(point1.x-0.5f)*(point1.x-0.5f));
	float d2=sqrt((point2.z-.25)*(point2.z-.25)+(point2.y-.25)*(point2.y-.25)+(point2.x-.25)*(point2.x-.25));				 
	d=lerp(1-d*2,turbperturb.w,blend);
	d2=lerp(1-d2*2,turbperturb.w,blend);
	float cloud=lerp(d,d2,implictBlend);
	Out.color.xyz=1-dot(2*(normal.xyz-.5),lightPos.xyz);
	
	if (cloud < 0.01f)
		Out.color.w = cloud;
	else
		Out.color.w = pow(cloud, 1.45);
	
	return Out;
	
}


technique TUber
{
    pass P0
    {          
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_3_0 PS();
    }
}