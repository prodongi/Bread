
float4x4 matWVP;
float4x4 matWVPPrev;
float3 positionOffset;	//파티클 박스의 옵셋 값
float3 velocity;
float  alpha;
float3 viewPosition;	// 카메라 위치
float3 sizeScale;		// float4(줌거리에 따른 넓이, 속도에 다른 크기, 0, 0)
float4 lighting;		// 라이트 칼라
float3 forward;			// 카메라의 z축 방향 벡터

struct VS_OUTPUT
{
	float4 pos		: POSITION;
	float2 uv		: TEXCOORD0;
	float4 color	: COLOR0;
};

struct VS_INPUT
{
	float3 pos		: POSITION;
	float2 uv		: TEXCOORD0;
};

struct PS_OUTPUT
{
	float4 color	: COLOR0;
};

texture diffuseTexture;
sampler diffuseSampler = sampler_state
{
    Texture = <diffuseTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;

    AddressU = Wrap;
    AddressV = Wrap;
};

#define PARTICLE_BOX	30.0f

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
	output.uv = input.uv;
	float3 position = input.pos;
	
	position.xyz -= positionOffset.xyz;
	position.xyz = fmod(position.xyz, PARTICLE_BOX);
	
	// 원점을 기준으로 이동
	position.xyz -= (PARTICLE_BOX/2);
	// 카메라의 앞쪽에 놓여지도록 이동
	position.xyz += forward.xyz;
	
	// 원점으로 부터 거리에 따른 칼라값 계산, 0 <= subtractColor <= 1
	float subtractColor = length(position.xyz)/PARTICLE_BOX;
	float distanceColorScale = 1.0f - subtractColor * subtractColor;
	
	float4 worldPositionBottom = float4(position.xyz + viewPosition.xyz, 1.0f);
	float4 worldPositionTop = float4(worldPositionBottom.xyz + velocity.xyz * sizeScale.y, 1.0f);
	
	// 프로젝션 좌표를 구한다
	float4 bottom = mul(worldPositionBottom, matWVP);
	float4 top = mul(worldPositionTop, matWVP);
	float4 topPrev = top;//mul(worldPositionTop, matWVPPrev);
	
	// 화면에서의 방향을 구한다.
	float2 dir = (top.xy/top.w) - (bottom.xy/bottom.w);
	float2 dirPrev = (topPrev.xy/topPrev.w) - (bottom.xy/bottom.w);
	
	float len = length(dir);
	float lenPrev = length(dirPrev);
	
	// 예전과 현재의 길이에 따른  칼라 스케일
	float lenColorScale = saturate(len/lenPrev);
	
	// 수직 방향을 구한다
	float2 dirPerp = normalize(float2(-dirPrev.y, dirPrev.x));
	
	// top과 bottom을 선택
	output.pos = lerp(topPrev, bottom, output.uv.y);
	// right와 left를 선택
	output.pos.xy += (0.5f - output.uv.x) * dirPerp * sizeScale.x;
	
	// 최종 칼라 결정
	output.color = lighting * distanceColorScale * lenColorScale * alpha;
		
	return output;
}

PS_OUTPUT PS(VS_OUTPUT input)
{
	PS_OUTPUT output;
	output.color = tex2D(diffuseSampler, input.uv) * input.color;
	return output;
}

technique T
{
    pass P0
    {
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();
    }
}