// -------------------------------------------------------------
// 투영텍스처 그림자
// 
// Copyright (c) 2003 IMAGIRE Takashi. All rights reserved.
// -------------------------------------------------------------

float4x4 mWVP;
float4x4 mWVPT;
float4	 vLightPos;
texture DecaleMap;
texture ShadowMap;

sampler DecaleMapSamp = sampler_state
{
    Texture = <DecaleMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Wrap;
    AddressV = Wrap;
};

sampler ShadowMapSamp = sampler_state
{
    Texture = <ShadowMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressV = Border;
    AddressU = Border;
};

struct VS_OUTPUT
{
	float4 Pos		: POSITION;
	float4 Color	: COLOR0;
	float2 TexDecale: TEXCOORD0;
	float4 TexShadow: TEXCOORD1;
};

struct VS_PROJ_OUTPUT
{
    float4 position   : POSITION;
    float2 uv0        : TEXCOORD0; 
};

struct PS_PROJ_OUTPUT
{
    float4 color: COLOR0;
};



VS_OUTPUT VS (
	  float4 Pos	: POSITION          // 정점위치
	, float4 Normal	: NORMAL            // 법선벡터
	, float2 Tex	: TEXCOORD0			// 텍스처좌표
){
	VS_OUTPUT Out;        // 출력데이터
	
	// 위치변환
	Out.Pos = mul( Pos, mWVP );
	
	Out.Color = max( dot(normalize(vLightPos.xyz-Pos.xyz), Normal), 0);
	
	// 텍스처좌표
	Out.TexDecale = Tex;
	
	// 텍스처좌표
	Out.TexShadow = mul( Pos, mWVPT );
	
	return Out;
}

float4 PS ( VS_OUTPUT In) : COLOR
{
	float4 decale = tex2D( DecaleMapSamp, In.TexDecale );
	float4 shadow = tex2Dproj( ShadowMapSamp, In.TexShadow );
	
	return decale * shadow;//(saturate(In.Color-0.5f*shadow)+0.3f);
}





VS_PROJ_OUTPUT vsProjShadow(float3 pos : POSITION,
						float2 uv0 : TEXCOORD0)
{
    VS_PROJ_OUTPUT Out;
    Out.position = mul(float4(pos, 1), mWVP);
    Out.uv0 = uv0;
    
    return Out;    
}



PS_PROJ_OUTPUT psProjShadow(VS_PROJ_OUTPUT In) 
{ 
    PS_PROJ_OUTPUT Out;
    Out.color = tex2D(DecaleMapSamp, In.uv0);
	Out.color.xyz = 0.5f;
    return Out;
}



technique TShader
{
    pass P0
    {
        // 셰이더
        VertexShader = compile vs_1_1 VS();
        PixelShader  = compile ps_2_0 PS();
    }
}

technique TProjShadow
{
	pass P0
    {
        // 셰이더
        VertexShader = compile vs_1_1 vsProjShadow();
        PixelShader  = compile ps_2_0 psProjShadow();
    }
}


