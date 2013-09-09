
#ifdef _SPECULAR_

void setVsSpecular(inout VS_OUTPUT Out, float3 N, float3 pos)
{
	Out.n = N;
	Out.eye = normalize(eyePos - pos.xyz);
}

float4 setPsSpecular(in VS_OUTPUT In)
{
	float3 L = -lightDir;
	float3 H = normalize(L + In.eye);
	float3 N = normalize(In.n);
	return 0.5f * pow(max(0.00001f, dot(N, H)), 8);
}

#endif