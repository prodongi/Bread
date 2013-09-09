
#ifdef _BUMP_

texture normalTexture;
sampler normalSampler = sampler_state
{
    Texture = <normalTexture>;
    MipFilter = NONE;
    MinFilter = LINEAR;
    MagFilter = LINEAR;

    AddressU = Wrap;
    AddressV = Wrap;
};

void setVsBump(inout VS_OUTPUT Out, float3 E, float3 L, float3 tangent, float3 binormal, float3 normal)
{
	Out.tanE.x = dot(E, tangent);
	Out.tanE.y = dot(E, binormal);
	Out.tanE.z = dot(E, normal);
	
	Out.tanL.x = dot(L, tangent);
	Out.tanL.y = dot(L, binormal);
	Out.tanL.z = dot(L, normal);
}

void setPsBump(in VS_OUTPUT In, inout PS_OUTPUT Out)
{
	float3 N = 2.0f * tex2D(normalSampler, In.uv0).xyz - 1.0f;
	float3 L = normalize(In.tanL);
	float3 R = reflect(-normalize(In.tanE), N);
	Out.color = Out.color * max(0.0f, dot(N, L)) + 0.3f * pow(max(0.0f, dot(R, L)), 8);
}



#endif