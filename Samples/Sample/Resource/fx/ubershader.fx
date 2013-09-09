
float4x4 matWorld;
float4x4 matWVP;
float3 ambient;
float fadeAlpha;

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
#ifdef _CSM_
	#include "fx\\shadowcsm.hlsl"
#endif
#ifdef _GPU_ANI_
	#include "fx\\gpuani.hlsl"
#endif
#ifdef _POM_
	#include "fx\\pom.hlsl"
#endif
#ifdef _VEGETATION_ANI_
	#include "fx\\vegetationani.hlsl"
#endif

VS_OUTPUT VS(VS_INPUT In)
{
    VS_OUTPUT Out;
	float4 _pos;
	float3 _normal;
	
#ifdef _GPU_ANI_
	skinAni(_pos, _normal, In.pos, In.normal, In.blendWeights, In.blendIndices);
#else
	_pos = In.pos;
	_normal = In.normal;
#endif

#ifdef _VEGETATION_ANI_
	vegetationAni(_pos);
#endif

	Out.position = mul(_pos, matWVP);

	float3 N = normalize(mul(_normal, (float3x3)matWorld));
	float3 L = -lightDir;
	
    Out.diffuse = max(0.0f, dot(N, L));
	Out.diffuse.xyz += ambient;
	Out.diffuse.xyz *= In.color.xyz;
	Out.diffuse.a = In.color.a* fadeAlpha;

	Out.uv0 = In.uv0;

#ifdef _BUMP_
	setVsBump(Out, eyePos-_pos, L, In.tangent, In.binormal, _normal);
#endif
#ifdef _POM_
	setVsPom(Out, _pos, -In.tangent, In.binormal, -_normal);
#endif

#ifdef _SPECULAR_
	setVsSpecular(Out, N, _pos);
#endif

#ifdef _SHADOWMAP_
	setVsShadowMap(Out, _pos);
#endif
#ifdef _PROJECTEDSHADOW_
	setVsProjectedShadow(Out, _pos);
#endif
#ifdef _CSM_
	setVsCsm(Out, _pos);
#endif

#ifdef _FOG_
	Out.fog = fog.x + Out.position.w * fog.y;
#endif
	
#ifdef _VISUAL_NORMAL_
	Out.diffuse.xyz = _normal;
	Out.diffuse.a = 1.0f;
#endif
  
    return Out;    
}

PS_OUTPUT PS(VS_OUTPUT In) 
{ 
    PS_OUTPUT Out;
	Out.color = tex2D(diffuseSampler, In.uv0) * In.diffuse;
	
#ifdef _VISUAL_NORMAL_
	Out.color = In.diffuse;
	return Out;
#endif

#ifdef _BUMP_
	setPsBump(In, Out);
#endif
#ifdef _POM_
	setPsPom(In, Out);
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
#ifdef _CSM_
	Out.color.xyz *= setPsCsm(In);
#endif

    return Out;
}

#ifdef _POM_
technique TUber
{
    pass P0
    {          
        VertexShader = compile vs_3_0 VS();
        PixelShader  = compile ps_3_0 PS();
    }
}
#else
technique TUber
{
    pass P0
    {          
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_3_0 PS();
    }
}
#endif













