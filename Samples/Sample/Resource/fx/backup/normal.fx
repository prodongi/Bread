
float4x4 matWVP;
float4x4 matWorld;
float4x4 matView;
texture baseTexture;
float mapW;
float mapH;
float p;

sampler baseSampler = sampler_state
{
	Texture = <baseTexture>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;

	AddressU = Wrap;
	AddressV = Wrap;
};


struct VS_OUTPUT
{
    float4 position   : POSITION;
    float4 normal     : COLOR0;
    float2 uv0 	      : TEXCOORD0;
    float4 color      : TEXCOORD1;
};

VS_OUTPUT VS(float3 pos : POSITION, 
	     float3 normal : NORMAL,
             float2 uv0 : TEXCOORD0)
{
    VS_OUTPUT Out;

    float4x4 matWV = mul(matWorld, matView);
    float3 P = mul(float4(pos, 1), (float4x3)matWV);
    float3 N = normalize(mul(normal, (float3x3)matWV));

    Out.position = mul(float4(pos, 1), matWVP);
    Out.normal = float4(N, length(P));
    Out.uv0 = uv0;
    Out.color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    return Out;    
}

struct PS_OUTPUT
{
    float4 color: COLOR0;
};

PS_OUTPUT PS(VS_OUTPUT In) 
{ 
    PS_OUTPUT Out;
    Out.color = In.normal;
    return Out;
}

float4 PS_Edge(VS_OUTPUT In) : COLOR
{
	float mask[9] = {-1, -1, -1,
			 -1, 8, -1,
			 -1, -1, -1 };
	float coord[3] = {-1, 0, 1};
	float4 color = 0;

	for (int i = 0; i < 9; ++i)
		color += mask[i]*(tex2D(baseSampler, In.uv0+float2(coord[i%3]/mapW, coord[i/3]/mapH)));

	color = pow(abs(color), p);
	color = 1 - color;

	float gray = color.r*0.3f + color.g*0.59f + color.b*0.11f;
	color = float4(gray, gray, gray, 1);

	//color = tex2D(baseSampler, Tex);

	return color;
}


technique T0
{
    pass P0
    {          
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();
    }

    pass P1
    {
	PixelShader = compile ps_2_0 PS_Edge();
    }
}
