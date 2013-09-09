
float4x4 matWorld;
float4x4 matWVP;
float3 ambient;

#include "fx\ubersampler.hlsl"
#include "fx\ubershared.hlsl"
#include "fx\uberstruct.hlsl"

#ifdef _SPECULAR_
	#include "fx\\specular.hlsl"
#endif
#ifdef _BUMP_
	#include "fx\\bump.hlsl"
#endif
#ifdef _SHADOWMAP_
	#include "fx\\shadowmap.hlsl"
#endif
#ifdef _PROJECTEDSHADOW_
	#include "fx\\projectedshadow.hlsl"
#endif


VS_OUTPUT VS(float4 pos : POSITION, float3 normal : NORMAL, float2 uv0 : TEXCOORD0
			 #ifdef _BUMP_
			  , float3 tangent : TANGENT0,
				float3 binormal : BINORMAL
			 #endif
			 )
{
    VS_OUTPUT Out;
	
	Out.position = mul(pos, matWVP);
	
	float3 L = -lightDir;
    float3 N = normalize(mul(normal, (float3x3)matWorld));
	
    Out.diffuse = max(0.0f, dot(N, L));
	Out.diffuse.xyz += ambient;
    Out.diffuse.a = 1.0f;
	Out.uv0 = uv0;
	
#ifdef _BUMP_
	setVsBump(Out, eyePos-pos, L, tangent, binormal, normal);
#endif

#ifdef _SPECULAR_
	setVsSpecular(Out, N, pos);
#endif

#ifdef _SHADOWMAP_
	setVsShadowMap(Out, pos);
#endif
#ifdef _PROJECTEDSHADOW_
	setVsProjectedShadow(Out, pos);
#endif

#ifdef _FOG_
	Out.fog = fog.x + Out.position.w * fog.y;
#endif
  
    return Out;    
}

PS_OUTPUT PS(VS_OUTPUT In) 
{ 
    PS_OUTPUT Out;
    Out.color = tex2D(diffuseSampler, In.uv0) * In.diffuse;

#ifdef _BUMP_
	setPsBump(In, Out);
#endif

#ifdef _SPECULAR_
	Out.color += setPsSpecular(In);
#endif

#ifdef _SHADOWMAP_
	Out.color.xyz *= setPsShadowMap(In);
#endif
#ifdef _PROJECTEDSHADOW_
	Out.color.xyz *= setPsProjectedShadow(In);
#endif

    return Out;
}

technique TUber
{
    pass P0
    {          
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();
    }
}














