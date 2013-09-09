
#ifdef _PROJECTEDSHADOW_

float4x4 matWVPT;
texture projectedShadowTexture;
sampler projectedShadowSampler = sampler_state
{
    Texture = <projectedShadowTexture>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressV = Border;
    AddressU = Border;
};

void setVsProjectedShadow(inout VS_OUTPUT Out, float4 pos)
{
	Out.shadowUv = mul(pos, matWVPT);
}

float setPsProjectedShadow(in VS_OUTPUT In)
{
	return tex2Dproj(projectedShadowSampler, In.shadowUv);
}



#endif