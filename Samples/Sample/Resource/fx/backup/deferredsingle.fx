
float4x4 matViewProjection;
float4x4 matWorldView;

float viewAspect;
float tanFOV;
float3 lightPos;
texture geomTexture;
texture normalTexture;
texture sceneTexture;
texture lightTexture;
float  invSqrLightRange;
float4  lightColor;
float lightRange;
float4 ambient = float4(0.5, 0.5, 0.5, 0.0);

sampler geomSampler = sampler_state
{
	Texture = <geomTexture>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	
	AddressU = Wrap;
	AddressV = Wrap;
};

sampler normalSampler = sampler_state
{
	Texture = <normalTexture>;
	MinFilter = POINT;
	MagFilter = POINT;
	
	AddressU = Wrap;
	AddressV = Wrap;
};

sampler sceneSampler = sampler_state
{
	Texture = <sceneTexture>;
	MinFilter = POINT;
	MagFilter = POINT;
	
	AddressU = Wrap;
	AddressV = Wrap;
};

sampler lightSampler = sampler_state
{
	Texture = <lightTexture>;
	MinFilter = POINT;
	MagFilter = POINT;
	
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VS_G_OUTPUT 
{
	float4 Pos:     POSITION;
	float3 Normal:  TEXCOORD0;
	float3 WorldPos:TEXCOORD1;
};

struct PS_G_INPUT
{
	float3 Normal:  TEXCOORD0;
	float3 WorldPos :TEXCOORD1;
};

struct VS_L_OUTPUT
{
	float4 pos          : POSITION0;
	float2 texCoord     : TEXCOORD0;
};

struct PS_L_INPUT
{
	float2 texCoord      : TEXCOORD0;
};


//
float3 restoreSamplerTo3dPos(float2 uv, float depth)
{
	// [0, 1] -> [-1, 1]
	float2 pos = float2(uv.x, -uv.y) * 2.0f - 1.0f;
	float3 eyeScreenRay = float3(pos.x*viewAspect, pos.y, tanFOV);
	return normalize(eyeScreenRay) * depth;
}

// normalSampler에서 normal.z를 복구해서 normal값을 완성한다
float3 restoreSamplerToNormal(float2 projPos)
{
	float3 normal;
	//[-1, 1] -> [0, 1]
	float2 uv = projPos * 0.5f + 0.5f;
	normal.xy =projPos;
	normal.z = -tex2D(normalSampler, uv);
	return normal;
}

float2 packFloat16(float depth)
{
	depth /= 4.0;
	float integer = floor(depth);
	float fraction = frac(depth);
	return float2(integer/256, fraction);
}

float unpackFloat16(float2 depth)
{
	float2 unpack = (1024.0, 4.0);
	return dot(unpack, depth);
}


VS_G_OUTPUT vsGeomState( float3 inPos: POSITION, float3 inNormal: NORMAL )
{
	VS_G_OUTPUT Out;
	
	Out.Pos    = mul(  float4(inPos, 1), matViewProjection);
	Out.Normal = mul( inNormal, (float3x3)matWorldView);
	float4 pp  = mul( float4(inPos, 1), matWorldView);
	Out.WorldPos = pp.xyz / pp.w;

	return Out;
}

float4 psGeomState( PS_G_INPUT Input ) : COLOR0
{
	float4 G_Buffer;
	
	G_Buffer.xy = normalize( Input.Normal ).xy; 
	G_Buffer.zw = packFloat16(length(Input.WorldPos));
//	G_Buffer.w = 0;
	
	return G_Buffer;
}


VS_L_OUTPUT vsLightState( float4 inPos: POSITION )
{
	VS_L_OUTPUT o = (VS_L_OUTPUT) 0;
	
	inPos.xy = sign( inPos.xy);
	o.pos = float4( inPos.xy, 0.0f, 1.0f);
	o.texCoord = float2(o.pos.x, -o.pos.y) * 0.5 + 0.5;
	
	return o;
}

float4 psDirectionLightState(PS_L_INPUT Input) : COLOR
{
	float4 sceneColor = tex2D(sceneSampler, Input.texCoord);
	float4 geomColor = tex2D( geomSampler, Input.texCoord );
	float3 _3dPos = restoreSamplerTo3dPos(Input.texCoord, geomColor.z);
	float3 normal = restoreSamplerToNormal(float2(geomColor.xy));
	
	float3 lightDir = lightPos - _3dPos;
	lightDir = normalize(lightDir);
	
	float  diffuseInfluence = dot(lightDir, normal);
	float4 color = lightColor * diffuseInfluence;
	return float4(color.xyz, 1.0f);
}

// 깊이값 복원이 잘 안되는거 같다
float4 psPointLightState(PS_L_INPUT Input) : COLOR
{
	//float4 sceneColor = tex2D(sceneSampler, Input.texCoord);
	float4 geomColor = tex2D( geomSampler, Input.texCoord );
	float depth = unpackFloat16(geomColor.zw);
	float3 _3dPos = restoreSamplerTo3dPos(Input.texCoord, depth);//geomColor.z);
	float3 normal = restoreSamplerToNormal(float2(geomColor.xy));
	
	float4 p = mul(float4(lightPos, 1), matWorldView);
	float3 _lightPos = p.xyz/p.w;
	float3 lightDir = (_lightPos - _3dPos)*invSqrLightRange;
	float   attenuation = saturate(1-dot(lightDir, lightDir));
	lightDir = normalize(lightDir);
	
	float  diffuseInfluence = dot(lightDir, normal)*attenuation;
	float4 color = lightColor * diffuseInfluence;
	//return float4(color.xyz, 1.0);
	
	float3 L = _lightPos - _3dPos;
	float _L = length(L)*0.001f;
	return _L;//float4(lightDir, 1.0f);
}

float4 psFinalState(float2 uv : TEXCOORD0) : COLOR
{
	float4 sceneColor = tex2D(sceneSampler, uv);
	float4 lightColor = tex2D(lightSampler, uv);
	return sceneColor * ( lightColor + ambient);
}



technique TGeom
{
    pass p0
	{
		VertexShader = compile vs_1_1 vsGeomState();
		PixelShader = compile ps_2_0 psGeomState();
	}
};

// direction light
technique TDirectionLight
{
	pass p0
	{
		VertexShader = compile vs_1_1 vsLightState();
		PixelShader = compile ps_2_0 psDirectionLightState();
	}
}

// point light
technique TPointLight
{
	pass p0
	{
		VertexShader = compile vs_1_1 vsLightState();
		PixelShader = compile ps_2_0 psPointLightState();
	}
}

// final
technique TFinal
{
	pass p0
	{
		VertexShader = NULL;
		PixelShader = compile ps_2_0 psFinalState();
	}
}












