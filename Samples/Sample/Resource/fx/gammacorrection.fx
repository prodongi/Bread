
texture baseTexture;
sampler baseSampler = sampler_state
{
    Texture = <baseTexture>;
    MipFilter = NONE;
    MinFilter = POINT;
    MagFilter = POINT;

    AddressU = Wrap;
    AddressV = Wrap;
};

texture gammaRampTexture;
sampler gammaRampSampler = sampler_state
{
    Texture = <gammaRampTexture>;
    MipFilter = NONE;
    MinFilter = POINT;
    MagFilter = POINT;

    AddressU = Wrap;
    AddressV = Wrap;
};

float4 psLuminance(float2 uv : TEXCOORD0) : COLOR
{ 
	float4 color = tex2D(baseSampler, uv);
	return 0.3f * color.r + 0.59f * color.g + 0.11f * color.b;
}

float4 psCorrection(float2 uv : TEXCOORD0) : COLOR
{
	float4 t0 = tex2D(baseSampler, uv);
	float4 t1 = tex2D(gammaRampSampler, t0.ar);
	float4 t2 = tex2D(gammaRampSampler, t0.gb);
	
	/*
		이걸 플어쓴게 아래 식이다
		float4 c0 = {1.0f, 0.0f, 0.0f, 0.0f};
		float4 c1 = {0.0f, 1.0f, 0.0f, 0.0f};
		float4 c2 = {0.0f, 0.0f, 1.0f, 0.0f};
		
		float4 r0;
		r0 = c0 * t1;
		r0 = (c1 * t2) + r0;
		r0 = (c2 * t2) + r0;
		r0.a = t0.a;
	*/
	
	float4 r0;
	r0.r = t1.r;
	r0.g = t2.g;
	r0.b = t2.b;
	r0.a = t0.a;
	return r0;
}

technique TLuminance
{
    pass P0
    {          
        VertexShader = null;
        PixelShader  = compile ps_2_0 psLuminance();
    }
}

technique TCorrection
{
	pass P0
	{
		VertexShader = null;
		PixelShader  = compile ps_2_0 psCorrection();
	}
}
