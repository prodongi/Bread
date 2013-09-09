
float4 PS(float2 tex : TEXCOORD0) : COLOR
{
	float3 normal;
	// [0, 1] -> [-1, 1]
	normal.xy = tex*2.0f - 1.0f;
	normal.z = sqrt(1.0f - dot(normal.xy, normal.xy));
	normal = normalize(normal);
	// [-1, 1] -> [0, 1]
	normal = normal*0.5f + 0.5f;
	
	return normal.z;
}

technique T0
{
    pass p0
	{
		VertexShader = NULL;
		PixelShader = compile ps_2_0 PS();
	}
};