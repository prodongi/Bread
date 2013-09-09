
struct VS_INPUT
{
	float4 pos 			: POSITION;
#ifdef _GPU_ANI_
	float4 blendWeights : BLENDWEIGHT;
	float4 blendIndices : BLENDINDICES;
#endif
	float3 normal 		: NORMAL; 
	float4 color 		: COLOR0; 
	float2 uv0 			: TEXCOORD0;
#if defined(_BUMP_) || defined(_POM_)
	float3 tangent 		: TANGENT0;
	float3 binormal 	: BINORMAL;
#endif
};

struct VS_OUTPUT
{
    float4 position   	: POSITION;
    float4 diffuse    	: COLOR0;
    float2 uv0        	: TEXCOORD0;

	/// shadow
#ifdef _SHADOWMAP_
	float4 shadowUv 	: TEXCOORD1;
	float4 depth       	: TEXCOORD2;
#endif
#ifdef _PROJECTEDSHADOW_
	float4 shadowUv		: TEXCOORD1;
#endif
#ifdef _CSM_
	float4 worldPos		: TEXCOORD1;
	float depth			: TEXCOORD2;
#endif
	
#ifdef _SPECULAR_
	float3 n			: TEXCOORD3;
	float3 eye			: TEXCOORD4;
#endif

#ifdef _BUMP_
	float3 tanL         : TEXCOORD5;	// 탄젠트 공간 반사 벡터
    float3 tanE	      	: TEXCOORD6;	// 탄젠트 공간 광원 벡터
#endif
#ifdef _POM_
    float3 vLightTS          : TEXCOORD3;   // light vector in tangent space, denormalized
    float3 vViewTS           : TEXCOORD4;   // view vector in tangent space, denormalized
    float2 vParallaxOffsetTS : TEXCOORD5;   // Parallax offset vector in tangent space
    float3 vNormalWS         : TEXCOORD6;   // Normal vector in world space
    float3 vViewWS           : TEXCOORD7;   // View vector in world space
#endif

#ifdef _FOG_
	float fog			: FOG;
#endif
};

struct PS_OUTPUT
{
    float4 color: COLOR0;
};

// diffuseNdotL의 볌위를 조정, 0.25~1.0 -> 0.0~1.0
// 면 법선과 정점 법선 계산과의 이질감을 보정해 주는 함수
// 테스트 해 봤는데, 차이점을 잘 모르겠음..
float computeDiffuseAdjustment(float diffuseNdotL)
{
	return saturate((diffuseNdotL * 1.25f) - 0.25f);
}
#ifdef _BUMP_
float computeDiffuseBumpAdjustment(float diffuseNdotL, float diffuseBumpNdotL)
{
	float adjustment = computeDiffuseAdjustment(diffuseBumpNdotL);
	adjustment *= 1.0f - (pow(1.0f - computeDiffuseAdjustment(diffuseNdotL), 8.0f));
	return saturate(adjustment);
}
#endif



