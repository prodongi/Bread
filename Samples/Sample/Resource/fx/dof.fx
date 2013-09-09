// ------------------------------------------------------------
// �ǻ��ɵ�
// 
// Copyright (c) 2003 IMAGIRE Takashi. All rights reserved.
// ------------------------------------------------------------

// ------------------------------------------------------------
// ��������
// ------------------------------------------------------------
float4x4 mWVP;
float   vCenter;
float   vScale;
float blurDu;
float blurDv;

// ------------------------------------------------------------
// �ؽ�ó
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
// �������̴����� �ȼ����̴��� �ѱ�� ������
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
// �������̴�(�������)
// ------------------------------------------------------------
VS_OUTPUT VS_pass0 (
      float4 Pos    : POSITION           // ������
     ,float4 Tex0   : TEXCOORD0	         // �ؽ�ó��ǥ
){
    VS_OUTPUT Out = (VS_OUTPUT)0;        // ��µ�����
    
    float4 pos = mul( Pos, mWVP );
    
	Out.Pos = pos;			// ��ġ��ǥ
	
	Out.Color = pos.z/pos.w;// �����п� ���̰�
	
    Out.Tex0 = Tex0;		// �ؽ�ó��ǥ

    return Out;
}
// ------------------------------------------------------------
// �ȼ����̴�(�������)
// ------------------------------------------------------------
float4 PS_pass0 (VS_OUTPUT In) : COLOR
{
	float4 Out = tex2D( FloorSamp, In.Tex0 );
	
	Out.a = In.Color.w;		// ���ļ��п� ���̰�
	
	return Out;
}



// ------------------------------------------------------------
// �ȼ����̴�(������)
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
// �ȼ����̴�(�ռ�)
// ------------------------------------------------------------
/*
PixelShader PS_pass3 = asm
{
    ps_1_1
    
    tex t0						// ������ �̹���
    tex t1						// �帴�� �̹���
    
    sub_x4     r0, t0.a,  c0    // ��Ŀ�� �߽��� c0
    mul_x4     r0, r0.a,  r0.a	// �����ؼ� ��ȣ���ֱ�
    mul_x4_sat r0, r0,    c1	// ���� ����
    
    lrp        r0, r0.a,  t1, t0// �����ռ�
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
// ��ũ��
// ------------------------------------------------------------
technique TBase
{
    pass P0 // �Ϲݷ����� + ���̷�����
    {		// �������
        VertexShader = compile vs_1_1 VS_pass0();
        PixelShader  = compile ps_2_0 PS_pass0();
    }
}

technique TBlur
{
    pass P0	// ������
    {
        VertexShader = null;//compile vs_1_1 VS_pass2();
        PixelShader  = compile ps_2_0 PS_pass2();
    }
}

technique TFinal
{
    pass P0	// �ռ�
    {
		VertexShader = null;
		PixelShader = compile ps_2_0 PS_pass3();
	}
}
