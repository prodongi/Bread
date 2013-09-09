
#ifdef _POM_

texture normalTexture;
sampler heightSampler = sampler_state
{
	Texture = <normalTexture>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

void setVsPom(inout VS_OUTPUT Out, float4 pos, float3 tangent, float3 binormal, float3 normal)
{	      
	float3 normalWS = mul(normal, (float3x3)matWorld);
	float3 tangentWS = mul(tangent, (float3x3)matWorld);
	float3 binormalWS = mul(binormal, (float3x3)matWorld);
	float3 positionWS = mul(pos, (float3x3)matWorld);

   // Propagate the world vertex normal through:   
   Out.vNormalWS = normalWS;
         
   // Compute and output the world view vector:
   //float3 vViewWS = mul( matViewInverse, float4( 0, 0, 0, 1)) - i.positionWS;
   float3 vViewWS = eyePos - positionWS;
   Out.vViewWS = vViewWS;

   // Compute denormalized light vector in world space:
   float3 vLightWS = lightDir;
   
   // Normalize the light and view vectors and transform it to the tangent space:
   float3x3 mWorldToTangent = float3x3( tangentWS, binormalWS, normalWS );
   
   // Propagate the view and the light vectors (in tangent space):
   //Out.vLightTS = mul( mWorldToTangent, vLightWS );
   Out.vLightTS = mul( vLightWS, mWorldToTangent);
   Out.vViewTS  = mul( mWorldToTangent, vViewWS  );
   
   // Compute the ray direction for intersecting the height field profile with 
   // current view ray. See the above paper for derivation of this computation.
     
   // Compute initial parallax displacement direction:
   float2 vParallaxDirection = normalize(  Out.vViewTS.xy );
   
   // The length of this vector determines the furthest amount of displacement:
   float fLength = length( Out.vViewTS );
   // 이부분 이해가 잘 안감
   float fParallaxLength = sqrt( fLength * fLength - Out.vViewTS.z * Out.vViewTS.z ) / Out.vViewTS.z; 
   
   // Compute the actual reverse parallax displacement vector:
   Out.vParallaxOffsetTS = vParallaxDirection * fParallaxLength;
   
   // Need to scale the amount of displacement to account for different height ranges
   // in height maps. This is controlled by an artist-editable parameter:
   float fHeightMapRange = 0.05f;
   Out.vParallaxOffsetTS *= fHeightMapRange;
}

float4 computeIllumination( float2 texCoord, float3 vLightTS, float3 vViewTS, float fOcclusionShadow )
{
   // Sample the normal from the normal map for the given texture sample:
   float3 vNormalTS = normalize( tex2D( heightSampler, texCoord ) * 2 - 1 );
   
   // Sample base map:
   float4 cBaseColor = tex2D( diffuseSampler, texCoord );
   
   // Compute diffuse color component:
   float4 cDiffuseColor = float4(1, 1, 1, 1);
   float4 cDiffuse = saturate( dot( vNormalTS, vLightTS )) * cDiffuseColor;
   
   // Compute specular component:
   float3 vReflectionTS = normalize( 2 * dot( vViewTS, vNormalTS ) * vNormalTS - vViewTS );
   
   float fRdotL = dot( vReflectionTS, vLightTS );
   
   float fSpecularExponent = 8;
   float4 cSpecularColor = float4(0.5, 0.5, 0.5, 1);
   float4 cAmbientColor = float4(0.5, 0.5, 0.5, 1);
   float4 cSpecular = saturate( pow( fRdotL, fSpecularExponent )) * cSpecularColor;
   
   float4 cFinalColor = (( cAmbientColor + cDiffuse ) * cBaseColor + cSpecular ) * fOcclusionShadow; 
   
   return cFinalColor;
 }   

void setPsPom(in VS_OUTPUT In, inout PS_OUTPUT Out)
{
//  Normalize the interpolated vectors:
   float3 vViewTS   = normalize( In.vViewTS  );
   float3 vViewWS   = normalize( In.vViewWS  );
   float3 vLightTS  = normalize( In.vLightTS );
   float3 vNormalWS = normalize( In.vNormalWS );
     
   float4 cResultColor = float4( 0, 0, 0, 1 );

   // Compute all the derivatives:
   float2 dx = ddx( In.uv0 );
   float2 dy = ddy( In.uv0 );
                  
   //===============================================//
   // Parallax occlusion mapping offset computation //
   //===============================================//

   // Utilize dynamic flow control to change the number of samples per ray 
   // depending on the viewing angle for the surface. Oblique angles require 
   // smaller step sizes to achieve more accurate precision for computing displacement.
   // We express the sampling rate as a linear function of the angle between 
   // the geometric normal and the view direction ray:
   int nMaxSamples = 50;
   int nMinSamples = 8;
   int nNumSteps = (int) lerp( nMaxSamples, nMinSamples, dot( vViewWS, vNormalWS ) );

   // Intersect the view ray with the height field profile along the direction of
   // the parallax offset ray (computed in the vertex shader. Note that the code is
   // designed specifically to take advantage of the dynamic flow control constructs
   // in HLSL and is very sensitive to specific syntax. When converting to other examples,
   // if still want to use dynamic flow control in the resulting assembly shader,
   // care must be applied.
   // 
   // In the below steps we approximate the height field profile as piecewise linear
   // curve. We find the pair of endpoints between which the intersection between the 
   // height field profile and the view ray is found and then compute line segment
   // intersection for the view ray and the line segment formed by the two endpoints.
   // This intersection is the displacement offset from the original texture coordinate.
   // See the above paper for more details about the process and derivation.
   //
   float fCurrHeight = 0.0;
   float fStepSize   = 1.0 / (float) nNumSteps;
   float fPrevHeight = 1.0;
   float fNextHeight = 0.0;

	int    nStepIndex = 0;
	bool   bCondition = true;

	float2 vTexOffsetPerStep = fStepSize * In.vParallaxOffsetTS;
	float2 vTexCurrentOffset = In.uv0;
	float  fCurrentBound     = 1.0;
	float  fParallaxAmount   = 0.0;

	float2 pt1 = 0;
	float2 pt2 = 0;

	float2 texOffset2 = 0;

	while ( nStepIndex < nNumSteps ) 
	{
		vTexCurrentOffset -= vTexOffsetPerStep;

		// Sample height map which in this case is stored in the alpha channel of the normal map:
		fCurrHeight = tex2Dgrad( heightSampler, vTexCurrentOffset, dx, dy ).a;

		fCurrentBound -= fStepSize;

		if ( fCurrHeight > fCurrentBound ) 
		{     
			pt1 = float2( fCurrentBound, fCurrHeight );				// 현재 높이 값
			pt2 = float2( fCurrentBound + fStepSize, fPrevHeight );	// 이전 높이 값

			texOffset2 = vTexCurrentOffset - vTexOffsetPerStep;

			nStepIndex = nNumSteps + 1;
		}
		else
		{
			nStepIndex++;
			fPrevHeight = fCurrHeight;
		}
	}   // End of while ( nStepIndex < nNumSteps )

	float fDelta2 = pt2.x - pt2.y;
	float fDelta1 = pt1.x - pt1.y;
	float fDenominator = fDelta2 - fDelta1;
   
	if ( fDenominator == 0.0f )
	{
      fParallaxAmount = 0.0f;
	}
	else
	{
	  // ?
      fParallaxAmount = (pt1.x * fDelta2 - pt2.x * fDelta1 ) / fDenominator;
	}
	float2 vParallaxOffset = In.vParallaxOffsetTS * (1 - fParallaxAmount );

	// The computed texture offset for the displaced point on the pseudo-extruded surface:
	float2 texSample = In.uv0 - vParallaxOffset;
   
	float fHeightMapRange = 1.0f;
	float2 vLightRayTS = vLightTS.xy * fHeightMapRange;
      
	// Compute the soft blurry shadows taking into account self-occlusion for features of the height
	// field:
   
	float fOcclusionShadow = 1.0f;
   
	bool isShadow = true;
      
	if (isShadow)
	{
		float fShadowSoftening = 0.5f;
		float sh0 =  tex2Dgrad( heightSampler, texSample, dx, dy ).a;
		float shA = (tex2Dgrad( heightSampler, texSample + vLightRayTS * 0.88, dx, dy ).a - sh0 - 0.88 ) *  1 * fShadowSoftening;
		float sh9 = (tex2Dgrad( heightSampler, texSample + vLightRayTS * 0.77, dx, dy ).a - sh0 - 0.77 ) *  2 * fShadowSoftening;
		float sh8 = (tex2Dgrad( heightSampler, texSample + vLightRayTS * 0.66, dx, dy ).a - sh0 - 0.66 ) *  4 * fShadowSoftening;
		float sh7 = (tex2Dgrad( heightSampler, texSample + vLightRayTS * 0.55, dx, dy ).a - sh0 - 0.55 ) *  6 * fShadowSoftening;
		float sh6 = (tex2Dgrad( heightSampler, texSample + vLightRayTS * 0.44, dx, dy ).a - sh0 - 0.44 ) *  8 * fShadowSoftening;
		float sh5 = (tex2Dgrad( heightSampler, texSample + vLightRayTS * 0.33, dx, dy ).a - sh0 - 0.33 ) * 10 * fShadowSoftening;
		float sh4 = (tex2Dgrad( heightSampler, texSample + vLightRayTS * 0.22, dx, dy ).a - sh0 - 0.22 ) * 12 * fShadowSoftening;
   
		// Compute the actual shadow strength:
		float fOcclusionShadow = 1 - max( max( max( max( max( max( shA, sh9 ), sh8 ), sh7 ), sh6 ), sh5 ), sh4 );
      
		// The previous computation overbrightens the image, let's adjust for that:
		fOcclusionShadow = fOcclusionShadow * 0.6 + 0.4; 
	}
               
	// Compute resulting color for the pixel:
	cResultColor = computeIllumination( texSample, vLightTS, vViewTS, fOcclusionShadow );
	Out.color = cResultColor;
}

#endif




























