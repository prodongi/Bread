
float4      tcLumOffsets[4];                // The offsets used by GreyScaleDownSample()
float4      tcDSOffsets[9];                 // The offsets used by DownSample()
texture 	baseTexture;

float fBrightPassThreshold;             // Values greater than this are accepted for the bright pass
float4 tcDownSampleOffsets[16];         // The sampling offsets used by 'DownSample' and 'BrightPass'
float HBloomWeights[9];                 // Description of the sampling distribution used by
float HBloomOffsets[9];                 // the HorizontalBlur() function
float VBloomWeights[9];                 // Description of the sampling distribution used by
float VBloomOffsets[9];                 // the VerticalBlur() function

texture originalTexture;   	// The HDR data
texture lumTexture;   		// The 1x1 luminance map
texture bloomTexture;		// The post processing results
float       fExposure;                          // A user configurable bias to under/over expose the image
float       fGaussianScalar;                    // Used in the post-processing, but also useful here
float       g_rcp_bloom_tex_w;                  // The reciprocal WIDTH of the texture in 'bloom'
float       g_rcp_bloom_tex_h;                  // The reciprocal HEIGHT of the texture in 'bloom'




sampler baseSampler = sampler_state
{
    Texture = <baseTexture>;
    MagFilter = LINEAR;
    MinFilter = LINEAR;

    AddressU = Wrap;
    AddressV = Wrap;
};
sampler originalSampler = sampler_state
{
    Texture = <originalTexture>;
    MagFilter = LINEAR;
    MinFilter = LINEAR;

    AddressU = Wrap;
    AddressV = Wrap;
};
sampler lumSampler = sampler_state
{
    Texture = <lumTexture>;
    MagFilter = LINEAR;
    MinFilter = LINEAR;

    AddressU = Wrap;
    AddressV = Wrap;
};
sampler bloomSampler = sampler_state
{
    Texture = <bloomTexture>;
    MagFilter = LINEAR;
    MinFilter = LINEAR;

    AddressU = Wrap;
    AddressV = Wrap;
};



//------------------------------------------------------------------
//  This entry point performs the basic first-pass when measuring
//  luminance of the HDR render target. It samples the HDR target
//  multiple times so as to compensate for the down-sampling and
//  subsequent loss of data.
//------------------------------------------------------------------
float4 GreyScaleDownSample(float2 t : TEXCOORD0 ) : COLOR0
{
    // Compute the average of the 4 necessary samples
        float average = 0.0f;
        float maximum = -1e20;
        float4 color = 0.0f;
        float3 WEIGHT = float3( 0.299f, 0.587f, 0.114f );
        
        for( int i = 0; i < 4; i++ )
        {
            color = tex2D( baseSampler, t + float2( tcLumOffsets[i].x, tcLumOffsets[i].y ) );
            
            // There are a number of ways we can try and convert the RGB value into
            // a single luminance value:
            
            // 1. Do a very simple mathematical average:
            //float GreyValue = dot( color.rgb, float3( 0.33f, 0.33f, 0.33f ) );
            
            // 2. Perform a more accurately weighted average:
            //float GreyValue = dot( color.rgb, WEIGHT );
            
            // 3. Take the maximum value of the incoming, same as computing the
            //    brightness/value for an HSV/HSB conversion:
            float GreyValue = max( color.r, max( color.g, color.b ) );
            
            // 4. Compute the luminance component as per the HSL colour space:
            //float GreyValue = 0.5f * ( max( color.r, max( color.g, color.b ) ) + min( color.r, min( color.g, color.b ) ) );
            
            // 5. Use the magnitude of the colour
            //float GreyValue = length( color.rgb );
                        
            maximum = max( maximum, GreyValue );
			// prodongi
            //average += (0.25f * log( 1e-5 + GreyValue )); //1e-5 necessary to stop the singularity at GreyValue=0
			average += GreyValue;
        }
        
		// prodongi
        //average = exp( average );
		average *= 0.25f;
        
    // Output the luminance to the render target
        return float4( average, maximum, 0.0f, 1.0f );       
}

//------------------------------------------------------------------
//  This entry point will, using a 3x3 set of reads will downsample
//  from one luminance map to another.
//------------------------------------------------------------------
float4 LuminanceDownSample( float2 t : TEXCOORD0 ) : COLOR0
{
    // Compute the average of the 10 necessary samples
        float4 color = 0.0f;
        float maximum = -1e20;
        float average = 0.0f;
        
        for( int i = 0; i < 9; i++ )
        {
            color = tex2D( baseSampler, t + float2( tcDSOffsets[i].x, tcDSOffsets[i].y ) );
            average += color.r;
            maximum = max( maximum, color.g );
        }
        
    // We've just taken 9 samples from the
    // high resolution texture, so compute the
    // actual average that is written to the
    // lower resolution texture (render target).
        average /= 9.0f;
        
    // Return the new average luminance
        return float4( average, maximum, 0.0f, 1.0f );
}




//------------------------------------------------------------------
// BRIGHT PASS AND 2x2 DOWN-SAMPLING PIXEL SHADER
//
// performs the 2x2 down sample, and then accepts any pixels
// that are greater or equal to the configured threshold
//------------------------------------------------------------------
float4 BrightPass( float2 t : TEXCOORD0 ) : COLOR
{
    
    float4 average = { 0.0f, 0.0f, 0.0f, 0.0f };

    // load in and combine the 4 samples from the source HDR texture
        for( int i = 0; i < 4; i++ )
        {
            average += tex2D( baseSampler, t + float2( tcDownSampleOffsets[i].x, tcDownSampleOffsets[i].y ) );
        }
            
        average *= 0.25f;

    // Determine the brightness of this particular pixel. As with the luminance calculations
    // there are 4 possible variations on this calculation:
    
    // 1. Do a very simple mathematical average:
        //float luminance = dot( average.rgb, float3( 0.33f, 0.33f, 0.33f ) );
    
    // 2. Perform a more accurately weighted average:
        //float luminance = dot( average.rgb, float3( 0.299f, 0.587f, 0.114f ) );
    
    // 3. Take the maximum value of the incoming, same as computing the
    //    brightness/value for an HSV/HSB conversion:
        float luminance = max( average.r, max( average.g, average.b ) );
    
    // 4. Compute the luminance component as per the HSL colour space:
        //float luminance = 0.5f * ( max( average.r, max( average.g, average.b ) ) + min( average.r, min( average.g, average.b ) ) );
    
    // 5. Use the magnitude of the colour
        //float luminance = length( average.rgb );
            
    // Determine whether this pixel passes the test...
        if( luminance < fBrightPassThreshold )
            average = float4( 0.0f, 0.0f, 0.0f, 1.0f );
    
    // Write the colour to the bright-pass render target
        return average;
    
}
    
    
    
//------------------------------------------------------------------
// 4x4 DOWN-SAMPLING PIXEL SHADER
//
// Samples the input texture 16x according to the provided offsets
// and then writes the average to the output texture
//------------------------------------------------------------------
float4 PostProcessDownSample( float2 t : TEXCOORD0 ) : COLOR
{
    float4 average = 0.0f;

    for( int i = 0; i < 16; i++ )
    {
        average += tex2D( baseSampler, t + float2(tcDownSampleOffsets[i].x, tcDownSampleOffsets[i].y) );
    }
        
    average /= 16.0f;
    return average;
}
    
    
    
//------------------------------------------------------------------
// HORIZONTAL BLUR
//
// Takes 9 samples from the down-sampled texture (4 either side and
// one central) biased by the provided weights. Different weight
// distributions will give more subtle/pronounced blurring.
//------------------------------------------------------------------
float4 HorizontalBlur( float2 t : TEXCOORD0 ) : COLOR
{

    float4 color = { 0.0f, 0.0f, 0.0f, 0.0f };
    
    for( int i = 0; i < 9; i++ )
    {
        color += (tex2D( baseSampler, t + float2( HBloomOffsets[i], 0.0f ) ) * HBloomWeights[i] );
    }
        
    return float4( color.rgb, 1.0f );
    
}
    
    
    
//------------------------------------------------------------------
// VERTICAL BLUR
//
// Takes 9 samples from the down-sampled texture (4 above/below and
// one central) biased by the provided weights. Different weight
// distributions will give more subtle/pronounced blurring.
//------------------------------------------------------------------
float4 VerticalBlur( float2 t : TEXCOORD0 ) : COLOR
{

    float4 color = { 0.0f, 0.0f, 0.0f, 0.0f };
    
    for( int i = 0; i < 9; i++ )
    {
        color += (tex2D( baseSampler, t + float2( 0.0f, VBloomOffsets[i] ) ) * VBloomWeights[i] );
    }
        
    return float4( color.rgb, 1.0f );
    
}


//------------------------------------------------------------------
//  SHADER ENTRY POINT
//------------------------------------------------------------------
float4 HDRFinal( float2 t : TEXCOORD0 ) : COLOR0
{

    // Read the HDR value that was computed as part of the original scene
        float4 c = tex2D( originalSampler, t );
    
    // Read the luminance value, target the centre of the texture
    // which will map to the only pixel in it!
        float4 l = tex2D( lumSampler, float2( 0.5f, 0.5f ) );
        
    // Compute the blur value using a bilinear filter
    // It is worth noting that if the hardware supports linear filtering of a
    // floating point render target that this step can probably be skipped.
        float xWeight = frac( t.x / g_rcp_bloom_tex_w ) - 0.5;
        float xDir = xWeight;
        xWeight = abs( xWeight );
        xDir /= xWeight;
        xDir *= g_rcp_bloom_tex_w;

        float yWeight = frac( t.y / g_rcp_bloom_tex_h ) - 0.5;
        float yDir = yWeight;
        yWeight = abs( yWeight );
        yDir /= yWeight;
        yDir *= g_rcp_bloom_tex_h;

    // sample the blur texture for the 4 relevant pixels, weighted accordingly
        float4 b = ((1.0f - xWeight) * (1.0f - yWeight))    * tex2D( bloomSampler, t );        
        b +=       (xWeight * (1.0f - yWeight))             * tex2D( bloomSampler, t + float2( xDir, 0.0f ) );
        b +=       (yWeight * (1.0f - xWeight))             * tex2D( bloomSampler, t + float2( 0.0f, yDir ) );
        b +=       (xWeight * yWeight)                      * tex2D( bloomSampler, t + float2( xDir, yDir ) );
            
    // Compute the actual colour:
        float4 final = c + 0.25f * b;
            
    // Reinhard's tone mapping equation (See Eqn#3 from 
    // "Photographic Tone Reproduction for Digital Images" for more details) is:
    //
    //      (      (   Lp    ))
    // Lp * (1.0f +(---------))
    //      (      ((Lm * Lm)))
    // -------------------------
    //         1.0f + Lp
    //
    // Lp is the luminance at the given point, this is computed using Eqn#2 from the above paper:
    //
    //        exposure
    //   Lp = -------- * HDRPixelIntensity
    //          l.r
    //
    // The exposure ("key" in the above paper) can be used to adjust the overall "balance" of 
    // the image. "l.r" is the average luminance across the scene, computed via the luminance
    // downsampling process. 'HDRPixelIntensity' is the measured brightness of the current pixel
    // being processed.
    
        float Lp = (fExposure / l.r) * max( final.r, max( final.g, final.b ) );
    
    // A slight difference is that we have a bloom component in the final image - this is *added* to the 
    // final result, therefore potentially increasing the maximum luminance across the whole image. 
    // For a bright area of the display, this factor should be the integral of the bloom distribution 
    // multipled by the maximum value. The integral of the gaussian distribution between [-1,+1] should 
    // be AT MOST 1.0; but the sample code adds a scalar to the front of this, making it a good enough
    // approximation to the *real* integral.
    
        float LmSqr = (l.g + fGaussianScalar * l.g) * (l.g + fGaussianScalar * l.g);
    
    // Compute Eqn#3:
        float toneScalar = ( Lp * ( 1.0f + ( Lp / ( LmSqr ) ) ) ) / ( 1.0f + Lp );
    
    // Tonemap the final outputted pixel:
        c = final * toneScalar;
    
    // Return the fully composed colour
        c.a = 1.0f;
        return c;

}


technique TLuminance1
{
    pass P0
    {          
        PixelShader  = compile ps_2_0 GreyScaleDownSample();
    }
}

technique TLuminance2
{
    pass P0
    {          
        PixelShader  = compile ps_2_0 LuminanceDownSample();
    }
}


technique TBrightPass
{
    pass P0
    {          
        PixelShader  = compile ps_2_0 BrightPass();
    }
}

technique TDownSample
{
    pass P0
    {          
        PixelShader  = compile ps_2_0 PostProcessDownSample();
    }
}

technique TBlurHorizontal
{
    pass P0
    {          
        PixelShader  = compile ps_2_0 HorizontalBlur();
    }
}

technique TBlurVertical
{
	pass P0
	{
		pixelShader = compile ps_2_0 VerticalBlur();
	}
}

technique THDRFinal
{
	pass P0
	{
		pixelShader = compile ps_2_0 HDRFinal();
	}
}
