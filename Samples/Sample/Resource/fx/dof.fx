// ------------------------------------------------------------
// 피사계심도
// 
// Copyright (c) 2003 IMAGIRE Takashi. All rights reserved.
// ------------------------------------------------------------

// ------------------------------------------------------------
// 전역변수
// ------------------------------------------------------------
float4x4 mWVP;
float   vCenter;
float   vScale;
float blurDu;
float blurDv;

// ------------------------------------------------------------
// 텍스처
// ------------------------------------------------------------
texture SrcTex;
sampler SrcSamp = sampler_state
{
    Texture = <SrcTex>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Clamp;
    AddressV = Clamp;
};
// ------------------------------------------------------------
sampler FloorSamp = sampler_state
{
    Texture = <SrcTex>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Wrap;
    AddressV = Wrap;
};
// ------------------------------------------------------------
texture BlurTex;
sampler BlurSamp = sampler_state
{
    Texture = <BlurTex>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Clamp;
    AddressV = Clamp;
};
// ------------------------------------------------------------
// 정점셰이더에서 픽셀셰이더로 넘기는 데이터
// ------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos			: POSITION;
    float4 Color		: COLOR0;
	float2 Tex0			: TEXCOORD0;
	float2 Tex1			: TEXCOORD1;
	float2 Tex2			: TEXCOORD2;
	float2 Tex3			: TEXCOORD3;
};

// ------------------------------------------------------------
// 정점셰이더(조명없음)
// ------------------------------------------------------------
VS_OUTPUT VS_pass0 (
      float4 Pos    : POSITION           // 모델정점
     ,float4 Tex0   : TEXCOORD0	         // 텍스처좌표
){
    VS_OUTPUT Out = (VS_OUTPUT)0;        // 출력데이터
    
    float4 pos = mul( Pos, mWVP );
    
	Out.Pos = pos;			// 위치좌표
	
	Out.Color = pos.z/pos.w;// 색성분에 깊이값
	
    Out.Tex0 = Tex0;		// 텍스처좌표

    return Out;
}
// ------------------------------------------------------------
// 픽셀셰이더(조명없음)
// ------------------------------------------------------------
float4 PS_pass0 (VS_OUTPUT In) : COLOR
{
	float4 Out = tex2D( FloorSamp, In.Tex0 );
	
	Out.a = In.Color.w;		// 알파성분에 깊이값
	
	return Out;
}



// ------------------------------------------------------------
// 픽셀셰이더(뭉개기)
// ------------------------------------------------------------
float4 PS_pass2 (float2 uv0 : TEXCOORD0) : COLOR
{	
	float4 col0 = tex2D( SrcSamp, uv0 );
	float4 col1 = tex2D( SrcSamp, float2(uv0.x+blurDu, uv0.y) );
	float4 col2 = tex2D( SrcSamp, float2(uv0.x, uv0.y+blurDv) );
	float4 col3 = tex2D( SrcSamp, float2(uv0.x+blurDu, uv0.y+blurDv) );
	
	return 0.25f*(col0+col1+col2+col3);
}


// ------------------------------------------------------------
// 픽셀셰이더(합성)
// ------------------------------------------------------------
/*
PixelShader PS_pass3 = asm
{
    ps_1_1
    
    tex t0						// 선명한 이미지
    tex t1						// 흐릿한 이미지
    
    sub_x4     r0, t0.a,  c0    // 포커스 중심을 c0
    mul_x4     r0, r0.a,  r0.a	// 제곱해서 부호없애기
    mul_x4_sat r0, r0,    c1	// 강도 조정
    
    lrp        r0, r0.a,  t1, t0// 선형합성
};
*/

float4 PS_pass3(float2 uv : TEXCOORD0) : COLOR0
{
	float4 t0 = tex2D(SrcSamp, uv);
	float4 t1 = tex2D(BlurSamp, uv);
	float r0;
	r0 = 4.0f * (t0.a - vCenter);
	r0 = 4.0f * pow(r0, 2);
	r0 = saturate(r0 * vScale * 4.0f);
	float4 color = lerp(t0, t1, r0);
	return color;
}
	
	
	


// ------------------------------------------------------------
// 테크닉
// ------------------------------------------------------------
technique TBase
{
    pass P0 // 일반렌더링 + 깊이렌더링
    {		// 조명없음
        VertexShader = compile vs_1_1 VS_pass0();
        PixelShader  = compile ps_2_0 PS_pass0();
    }
}

technique TBlur
{
    pass P0	// 뭉개기
    {
        VertexShader = null;//compile vs_1_1 VS_pass2();
        PixelShader  = compile ps_2_0 PS_pass2();
    }
}

technique TFinal
{
    pass P0	// 합성
    {
		VertexShader = null;
		PixelShader = compile ps_2_0 PS_pass3();
	}
}
