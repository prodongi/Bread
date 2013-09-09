
shared float3 eyePos;
shared float3 lightDir;
shared float4x4 matVP;

#ifdef _FOG_
	shared float2 fog;
#endif