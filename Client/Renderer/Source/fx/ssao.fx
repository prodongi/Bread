float4 viewDimensions;
float4 params = float4(0.5f, 0.5f, 0.3f, 200.0f);
texture depthTexture;
texture noiseTexture;

sampler depthSampler = sampler_state
{
	Texture = <depthTexture>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = None;
	
	AddressU = Clamp;
	AddressV = Clamp;
	AddressW = Clamp;
};

sampler noiseSampler = sampler_state
{
	Texture = <noiseTexture>;
	MinFilter = Point;
	MagFilter = Point;
	MipFilter = None;
	
	AddressU = Wrap;
	AddressV = Wrap;
	AddressW = Wrap;
};

struct VS_OUTPUT
{
	float4 pos : POSITION;
	float2 uv  : TEXCOORD0;
};

VS_OUTPUT vs(float3 pos : POSITION,
             float2 uv : TEXCOORD0)
{
	VS_OUTPUT Out;
	Out.pos = float4(pos.x, pos.y, pos.z, 1.0f);
	Out.uv = uv;
	return Out;
}

float4 ps(VS_OUTPUT In) : COLOR0
{
	const float3 avKernel[8] =
	{
		normalize( float3(  1, 1, 1 ) ) * 0.125f,
		normalize( float3( -1,-1,-1 ) ) * 0.250f,
		normalize( float3( -1,-1, 1 ) ) * 0.375f,
		normalize( float3( -1, 1,-1 ) ) * 0.500f,
		normalize( float3( -1, 1 ,1 ) ) * 0.625f,
		normalize( float3(  1,-1,-1 ) ) * 0.750f,
		normalize( float3(  1,-1, 1 ) ) * 0.875f,
		normalize( float3(  1, 1,-1 ) ) * 1.000f
	};
	
	float3 random = tex2D(noiseSampler, In.uv.xy*10.0f).xyz;
	random = random * 2.0f - 1.0f;
	
	float radius = params.y;
	float pixelDepth = tex2D(depthSampler, In.uv).r;
	float depth = pixelDepth * viewDimensions.z;
	float3 kernelScale = float3(radius/depth, radius/depth, radius/viewDimensions.z);
	float occlusion = 0.0f;
	
	#define NUM_PASSES 2

	for ( int j = 0; j < NUM_PASSES; j++ )
	{
		float3 random = tex2D(noiseSampler, In.uv.xy * ( 7.0f + (float)j ) ).xyz;
		random = random * 2.0f - 1.0f;

		for ( int i = 0; i < 8; i++ )
		{
			float3 rotatedKernel = reflect( avKernel[i], random ) * kernelScale;

			float sampleDepth = tex2D(depthSampler, rotatedKernel.xy + In.uv ).r;
			float delta = max( sampleDepth - pixelDepth + rotatedKernel.z, 0.0f );
			float range = abs( delta ) / ( kernelScale.z * params.z );
			occlusion += lerp( /*saturate*/( delta * params.w ), params.x, saturate( range ) );
		}
	}

	occlusion = occlusion / ( NUM_PASSES * 8.0f );
	
	float4 color;
	color = occlusion;
	color = lerp( 0.1f, 0.6, saturate( color.x ) );
	//color = depth;
	return color;
}

technique T
{
    pass P0
    {          
        VertexShader = compile vs_2_0 vs();
        PixelShader  = compile ps_3_0 ps();
    }
}