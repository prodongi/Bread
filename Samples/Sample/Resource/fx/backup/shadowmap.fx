
float4x4 mWVP;		// scene : world-view-proj
float4x4 mWLP;		// light : world-light view-light perspective
float4x4 mWLPB;		// light : mWLP-light proj
float4   vCol;		//
float4	 vLightDir;	//
float DepthBias;
texture ShadowMap;
texture DecaleMap;

sampler ShadowMapSamp = sampler_state
{
    Texture = <ShadowMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Border;
    AddressV = Border;
};
sampler DecaleMapSamp = sampler_state
{
    Texture = <DecaleMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Wrap;
    AddressV = Wrap;
};

struct VS_O_DEPTH
{
	float4 pos	: POSITION;
	float2 depth : TEXCOORD0;
};
struct VS_O_SCENE
{
    float4 Pos			: POSITION;
	float4 Diffuse		: COLOR0;
	float4 Ambient		: COLOR1;
	float4 ShadowMapUV	: TEXCOORD0;
	float4 Depth		: TEXCOORD1;
	float2 DecaleTex	: TEXCOORD2;
};



VS_O_DEPTH vsDepth(float4 Pos    : POSITION,
				   float3 Normal : NORMAL)
{
    VS_O_DEPTH Out = (VS_O_DEPTH)0;
	Out.pos = mul( Pos, mWLP );
    Out.depth.xy = Out.pos.zw;
    return Out;
}
float4 psDepth(VS_O_DEPTH In) : COLOR
{ 
    float depth = In.depth.x/In.depth.y;
	float bias = depth/DepthBias;
	return depth + bias;
}



VS_O_SCENE vsScene(float4 Pos    : POSITION,
				  float3 Normal : NORMAL,
				  float2 Tex    : TEXCOORD0)
{
    VS_O_SCENE Out = (VS_O_SCENE)0;
	float4	uv;
	
    Out.Pos = mul(Pos, mWVP);
	Out.Diffuse = vCol * max( dot(vLightDir, Normal), 0);
	Out.Ambient = vCol * 0.3f;
	Out.ShadowMapUV = mul(Pos, mWLPB);
	Out.Depth = mul(Pos, mWLP);
	Out.DecaleTex = Tex;
		
    return Out;
}
float4 psScene(VS_O_SCENE In) : COLOR
{
	float  shadow = tex2Dproj( ShadowMapSamp, In.ShadowMapUV );
	float4 decale = tex2D( DecaleMapSamp, In.DecaleTex );
	
	float sceneDepthW = max(In.Depth.w, 1);
	float sceneDepth = In.Depth.z;
	float shadowColor = (sceneDepth < shadow * sceneDepthW) ? 1.0f : 0.5f;

	float4 color = In.Ambient + shadowColor * In.Diffuse;
    return color * decale;
} 




technique TDepth
{
    pass P0
    {
        VertexShader = compile vs_1_1 vsDepth();
        PixelShader  = compile ps_2_0 psDepth();
    }
}
technique TScene
{
	pass p0
	{
		VertexShader = compile vs_1_1 vsScene();
		PixelShader  = compile ps_2_0 psScene();
	}
}










