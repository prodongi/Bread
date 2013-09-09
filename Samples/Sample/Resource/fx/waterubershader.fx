
float4x4 matWorld;
float4x4 matWVP;
float3 ambient;
float time = 1.0f;

float2 textureScale = float2(8.0f, 4.0f);
float waveFreq = 0.0065f;
float waveAmp = 8.0f;
float bumpScale = 0.2f;
float2 bumpSpeed = float2(-0.1f, 0.0f);
float4 deepColor = float4(0.0f, 0.0f, 0.1f, 1.0f);
float4 shallowColor = float4(0.0f, 0.3f, 0.4f, 1.0f);
float waterAmount = 0.3f;

float4 reflectionColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
float reflectionAmount = 1.0f;
float reflectionBlur = 0.0f;

float fresnelPower = 4.0f;
float fresnelBias = 0.2f;
float hdrMultiplier = 3.0f;

#include "fx\ubershared.hlsl"

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

texture normalTexture;
sampler2D normalSampler = sampler_state
{
	Texture = <normalTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;

    AddressU = Wrap;
    AddressV = Wrap;
};

texture environmentTexture;
samplerCUBE environmentSampler = sampler_state
{
	Texture = <environmentTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;

    AddressU = Clamp;
    AddressV = Clamp;
};

struct Wave
{
	float freq;
	float amp;
	float phase;	// 위상 : 동일한 주기(주파수)에서 변위/차이/어긋남 등을 표현하는 양
	float2 dir;
};

#define NWAVES 2
Wave wave[NWAVES] = {
        { 1.0, 1.0, 0.5, float2(-1, 0) },
        { 2.0, 0.5, 1.7, float2(-0.7, 0.7) }
    };

struct VS_INPUT
{
	float4 pos 			: POSITION;
	//float4 color 		: COLOR0; 
	float2 uv0 			: TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 pos 		  	: POSITION;
    //float4 color    	: COLOR0;
	float2 texCoord		: TEXCOORD0;
	float3 rotMatrix1 : TEXCOORD1; // first row of the 3x3 transform 
    float3 rotMatrix2 : TEXCOORD2; // second row of the 3x3 transform 
    float3 rotMatrix3 : TEXCOORD3; // third row of the 3x3 transform 
    float2 bumpCoord0 : TEXCOORD4;
    float2 bumpCoord1 : TEXCOORD5;
    float2 bumpCoord2 : TEXCOORD6;

    float3 eyeVector : TEXCOORD7;

#ifdef _FOG_
	float fog			: FOG;
#endif
};

struct PS_OUTPUT
{
    float4 color: COLOR0;
};

void sumWaves(Wave wave, inout float4 P, inout float ddx, inout float ddy)
{
	float angle = dot(wave.dir, P.xz) * wave.freq + time * wave.phase;
	P.y += wave.amp * sin(angle);
	float deriv = wave.freq * wave.amp * cos(angle);
	ddx += deriv * wave.dir.x;
	ddy += deriv * wave.dir.y;
}

VS_OUTPUT VS(VS_INPUT In)
{
	VS_OUTPUT Out;

    wave[0].freq = waveFreq;
    wave[0].amp = waveAmp;

    wave[1].freq = waveFreq * 3.0;
    wave[1].amp = waveAmp * 0.33;

    float4 P = In.pos;
    
     // sum waves
    float ddx = 0.0, ddy = 0.0;
    for(int i = 0; i<NWAVES; ++i)
    {
		sumWaves(wave[i], P, ddx, ddy);
    }
    
	// compute tangent basis
    float3 B = float3(1, ddx, 0);
    float3 T = float3(0, ddy, 1);
    float3 N = float3(ddx, 1, ddy);
	
    Out.pos = mul(P, matWVP);
	Out.texCoord = In.uv0 * textureScale;
    
    // calculate texture coordinates for normal map lookup
	float2 uv = In.uv0 * textureScale;
	float2 bump_t = time * bumpSpeed;
    Out.bumpCoord0.xy = uv + bump_t;
    Out.bumpCoord1.xy = uv * 2.0f + bump_t * 4.0f;
    Out.bumpCoord2.xy = uv * 4.0f + bump_t * 8.0f;
	
	// compute the 3x3 tranform from tangent space to object space
	float3x3 objToTangentSpace;
	// first rows are the tangent and binormal scaled by the bump scale
	objToTangentSpace[0] = bumpScale * normalize(B);
	objToTangentSpace[1] = bumpScale * normalize(T);
	objToTangentSpace[2] = normalize(N);

	Out.rotMatrix1.xyz = mul(matWorld[0].xyz, objToTangentSpace);
	Out.rotMatrix2.xyz = mul(matWorld[1].xyz, objToTangentSpace);
	Out.rotMatrix3.xyz = mul(matWorld[2].xyz, objToTangentSpace);
  
 // compute the eye vector (going from shaded point to eye) in cube space
	float4 worldPos = mul(P, matWorld);
	Out.eyeVector = eyePos - worldPos; // eye position in vertex space
	
	return Out;
}

PS_OUTPUT PS(VS_OUTPUT In) 
{ 
    PS_OUTPUT Out;
	//Out.color = In.color;
	float4 t0 = tex2D(normalSampler, In.bumpCoord0) * 2.0 - 1.0;
    float4 t1 = tex2D(normalSampler, In.bumpCoord1) * 2.0 - 1.0;
    float4 t2 = tex2D(normalSampler, In.bumpCoord2) * 2.0 - 1.0;
    float3 N = t0.xyz + t1.xyz + t2.xyz;
    
    float3x3 m; // tangent to world matrix
    m[0] = In.rotMatrix1;
    m[1] = In.rotMatrix2;
    m[2] = In.rotMatrix3;
    
    N = normalize( mul( N, m ) );
    
    // reflection
    float3 E = normalize(In.eyeVector);
    float4 R;
    R.xyz = reflect(-E, N);
	R.w = reflectionBlur;
	
    float4 reflection = texCUBEbias(environmentSampler, R);
    // hdr effect (multiplier in alpha channel)
    reflection.rgb *= (1.0 + reflection.a*hdrMultiplier);

    // fresnel
    float facing = 1.0 - max(dot(E, N), 0);
    float fresnel = fresnelBias + (1.0-fresnelBias)*pow(facing, fresnelPower);

    float4 waterColor = lerp(deepColor, shallowColor, facing);
	Out.color = waterColor*waterAmount + reflection*reflectionColor*reflectionAmount*fresnel;
	
	return Out;
}
	
technique TUber
{
    pass P0
    {          
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_3_0 PS();
    }
}
