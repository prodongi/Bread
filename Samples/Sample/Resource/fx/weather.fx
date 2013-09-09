
float4x4 matWVP;
float4x4 matWVPPrev;
float3 positionOffset;	//��ƼŬ �ڽ��� �ɼ� ��
float3 velocity;
float  alpha;
float3 viewPosition;	// ī�޶� ��ġ
float3 sizeScale;		// float4(�ܰŸ��� ���� ����, �ӵ��� �ٸ� ũ��, 0, 0)
float4 lighting;		// ����Ʈ Į��
float3 forward;			// ī�޶��� z�� ���� ����

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
	
	// ������ �������� �̵�
	position.xyz -= (PARTICLE_BOX/2);
	// ī�޶��� ���ʿ� ���������� �̵�
	position.xyz += forward.xyz;
	
	// �������� ���� �Ÿ��� ���� Į�� ���, 0 <= subtractColor <= 1
	float subtractColor = length(position.xyz)/PARTICLE_BOX;
	float distanceColorScale = 1.0f - subtractColor * subtractColor;
	
	float4 worldPositionBottom = float4(position.xyz + viewPosition.xyz, 1.0f);
	float4 worldPositionTop = float4(worldPositionBottom.xyz + velocity.xyz * sizeScale.y, 1.0f);
	
	// �������� ��ǥ�� ���Ѵ�
	float4 bottom = mul(worldPositionBottom, matWVP);
	float4 top = mul(worldPositionTop, matWVP);
	float4 topPrev = top;//mul(worldPositionTop, matWVPPrev);
	
	// ȭ�鿡���� ������ ���Ѵ�.
	float2 dir = (top.xy/top.w) - (bottom.xy/bottom.w);
	float2 dirPrev = (topPrev.xy/topPrev.w) - (bottom.xy/bottom.w);
	
	float len = length(dir);
	float lenPrev = length(dirPrev);
	
	// ������ ������ ���̿� ����  Į�� ������
	float lenColorScale = saturate(len/lenPrev);
	
	// ���� ������ ���Ѵ�
	float2 dirPerp = normalize(float2(-dirPrev.y, dirPrev.x));
	
	// top�� bottom�� ����
	output.pos = lerp(topPrev, bottom, output.uv.y);
	// right�� left�� ����
	output.pos.xy += (0.5f - output.uv.x) * dirPerp * sizeScale.x;
	
	// ���� Į�� ����
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