
#ifdef _CSM_

float4x4 matWLPT[4];
float depthBias;
float4 cascadedZNear;

texture shadowCsmTexture0;
texture shadowCsmTexture1;
texture shadowCsmTexture2;
texture shadowCsmTexture3;

sampler shadowCsmSampler0 = sampler_state
{
    Texture = <shadowCsmTexture0>;
    MinFilter = POINT;
    MagFilter = POINT;
    MipFilter = NONE;

    AddressV = Border;
    AddressU = Border;
};

sampler shadowCsmSampler1 = sampler_state
{
    Texture = <shadowCsmTexture1>;
    MinFilter = POINT;
    MagFilter = POINT;
    MipFilter = NONE;

    AddressV = Border;
    AddressU = Border;
};

sampler shadowCsmSampler2 = sampler_state
{
    Texture = <shadowCsmTexture2>;
    MinFilter = POINT;
    MagFilter = POINT;
    MipFilter = NONE;

    AddressV = Border;
    AddressU = Border;
};

sampler shadowCsmSampler3 = sampler_state
{
    Texture = <shadowCsmTexture3>;
    MinFilter = POINT;
    MagFilter = POINT;
    MipFilter = NONE;

    AddressV = Border;
    AddressU = Border;
};



void setVsCsm(inout VS_OUTPUT Out, float4 pos)
{
	Out.worldPos = pos;
	Out.depth = Out.position.w;
}

float3 setPsCsm(in VS_OUTPUT In)
{
	float4 checkDepth = (In.depth > cascadedZNear);
	float cascadedIdx = dot(checkDepth, 1.0f) - 1;
	
	float4 shadowUv = mul(float4(In.worldPos.xyz, 1.0f), matWLPT[cascadedIdx]);
	float texDepth;
	if (cascadedIdx == 0)	texDepth = tex2D(shadowCsmSampler0, shadowUv);
	else if (cascadedIdx == 1) texDepth = tex2D(shadowCsmSampler1, shadowUv);
	else if (cascadedIdx == 2) texDepth = tex2D(shadowCsmSampler2, shadowUv);
	else if (cascadedIdx == 3) texDepth = tex2D(shadowCsmSampler3, shadowUv);
	
	if (texDepth == 0.0f)
		return 1.0f;
	
	float sceneDepth = saturate((shadowUv.z / shadowUv.w) - depthBias);
	float shadowColor = (sceneDepth > texDepth) ? 0.8f : 1.0f;
	
	return shadowColor;
}

#endif