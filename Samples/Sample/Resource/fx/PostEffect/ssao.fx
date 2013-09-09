
// shaderx 7에서 참조 함
texture depthTexture;
texture rotTexture;
float farDistance;
float screenSize;

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

sampler rotSampler = sampler_state
{
	Texture = <rotTexture>;
	MinFilter = None;
	MagFilter = None;
	MipFilter = None;
	
	AddressU = Clamp;
	AddressV = Clamp;
	AddressW = Clamp;
};

struct VS_OUTPUT
{
	float4 pos : POSITION;
	float2 uv  : TEXCOORD0;
};

float4 ps(VS_OUTPUT In) : COLOR0
{
	// rot 랜덤 샘플러가 4x4이기 때문에 4픽셀 단위로 타일링 되게 한다.
	float2 rotationTC = In.uv * screenSize / 4;
	float3 vRotation = 2 * tex2D(rotSampler, rotationTC).rgb - 1;
	
	// vRotation의 회전 매트릭스 계산
	float3x3 rotMat;
	float h = 1/(1 + vRotation.z);
	rotMat._m00 = h * vRotation.y*vRotation.y + vRotation.z;
	rotMat._m01 = -h*vRotation.y*vRotation.x;
	rotMat._m02 = -vRotation.x;
	rotMat._m10 = -h*vRotation.y*vRotation.x;
	rotMat._m11 = h * vRotation.x*vRotation.x + vRotation.z;
	rotMat._m12 = -vRotation.y;
	rotMat._m20 = vRotation.x;
	rotMat._m21 = vRotation.y;
	rotMat._m22 = vRotation.z;
		
	float sceneDepthP = tex2D(depthSampler, In.uv).r * farDistance;
	
	int samplerNum = 16;
	float offsetScale = 0.001f;
	// 왜 2.4에서 샘플링 개수를 나누는가?
	float offsetScaleStep = 1 + 2.4/samplerNum;
	float accessibility = 0.0f;
	
	for (int i = 0; i < samplerNum/8; ++i)
	{
		for (int x = -1; x <= 1; x+=2)
		{
			for (int y = -1; y <= 1; y += 2)
			{
				for (int z = -1; z <= 1; z += 2)
				{
					float3 offset = normalize(float3(x, y, z)) * (offsetScale *= offsetScaleStep);
					
					float3 vRotatedOffset = mul(offset, rotMat);
					
					float3 samplePos = float3(In.uv, sceneDepthP);
					samplePos += float3(vRotatedOffset.xy, vRotatedOffset.z * sceneDepthP * 2);
					float sceneDepthS = tex2D(depthSampler, samplePos.xy) * farDistance;
					float rangeIsInvalid = saturate(((sceneDepthP - sceneDepthS)/sceneDepthS));
					
					if (sceneDepthP == sceneDepthS)
						rangeIsInvalid = 1.0f;
					
					accessibility += lerp(sceneDepthS > samplePos.z, 0.5, rangeIsInvalid);
				}				
			}
		}
	}
	
	accessibility = accessibility/samplerNum;
	
	return saturate(accessibility * accessibility + accessibility);
}

technique T
{
    pass P0
    {          
        VertexShader = NULL;
        PixelShader  = compile ps_3_0 ps();
    }
}