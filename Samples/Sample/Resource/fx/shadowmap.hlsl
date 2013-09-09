
#ifdef _SHADOWMAP_

float4x4 matWLPB;
float4x4 matWLP;
	
texture shadowMapDepthTexture;
sampler shadowMapDepthSampler = sampler_state
{
	Texture = <shadowMapDepthTexture>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Border;
	AddressV = Border;
};

void setVsShadowMap(inout VS_OUTPUT Out, float4 pos)
{
	Out.shadowUv = mul(pos, matWLPB);
	Out.depth = mul(pos, matWLP);
}

float setPsShadowMap(in VS_OUTPUT In)
{
	float shadow = tex2Dproj(shadowMapDepthSampler, In.shadowUv );
	float sceneDepthW = max(In.depth.w, 1);
	float sceneDepth = In.depth.z;
	float shadowColor = (sceneDepth < shadow * sceneDepthW) ? 0.5f : 1.0f;
	return shadowColor;
}


#endif