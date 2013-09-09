
#ifndef _PostEffectShaderList_h_
#define _PostEffectShaderList_h_

#include "BreadShader.h"
#include "BreadMatrix4.h"

namespace Bread
{
	/*
	 *	
	 */
	class cPostEffectMonoChromeShader : public cShader
	{
	public:
		cPostEffectMonoChromeShader() {}
		virtual ~cPostEffectMonoChromeShader() {}
		virtual void registHandle();
		
	public:
		D3DXHANDLE m_t;
		D3DXHANDLE m_luminance;
		D3DXHANDLE m_sceneTexture;
	};
	/*
	 *	
	 */
	class cPostEffectSepiaShader : public cShader
	{
	public:
		cPostEffectSepiaShader() {}
		virtual ~cPostEffectSepiaShader() {}
		virtual void registHandle();
		
	public:
		D3DXHANDLE m_t;
		D3DXHANDLE m_intensityConverter;
		D3DXHANDLE m_sepiaConvert;
		D3DXHANDLE m_sceneTexture;
	};
	/*
	 *	
	 */
	class cPostEffectInverseShader : public cShader
	{
	public:
		cPostEffectInverseShader() {}
		virtual ~cPostEffectInverseShader() {}
		virtual void registHandle();
		
	public:
		D3DXHANDLE m_t;
		D3DXHANDLE m_sceneTexture;
	};
	/*
	 *	
	 */
	class cPostEffectGBlurVShader : public cShader
	{
	public:
		cPostEffectGBlurVShader() {}
		virtual ~cPostEffectGBlurVShader() {}
		virtual void registHandle();
		
	public:
		D3DXHANDLE m_t;
		D3DXHANDLE m_sceneTexture;
		D3DXHANDLE m_samplingV;
	};
	/*
	 *	
	 */
	class cPostEffectGBlurHShader : public cShader
	{
	public:
		cPostEffectGBlurHShader() {}
		virtual ~cPostEffectGBlurHShader() {}
		virtual void registHandle();
		
	public:
		D3DXHANDLE m_t;
		D3DXHANDLE m_sceneTexture;
		D3DXHANDLE m_samplingU;
	};
	/*
	 *	
	 */
	class cPostEffectBloomVShader : public cShader
	{
	public:
		cPostEffectBloomVShader() {}
		virtual ~cPostEffectBloomVShader() {}
		virtual void registHandle();
		
	public:
		D3DXHANDLE m_t;
		D3DXHANDLE m_sceneTexture;
		D3DXHANDLE m_samplingV;
	};
	/*
	 *	
	 */
	class cPostEffectBloomHShader : public cShader
	{
	public:
		cPostEffectBloomHShader() {}
		virtual ~cPostEffectBloomHShader() {}
		virtual void registHandle();
		
	public:
		D3DXHANDLE m_t;
		D3DXHANDLE m_sceneTexture;
		D3DXHANDLE m_samplingU;
	};
	/*
	 *	
	 */
	class cPostEffectScaleHalfShader : public cShader
	{
	public:
		cPostEffectScaleHalfShader() {}
		virtual ~cPostEffectScaleHalfShader() {}
		virtual void registHandle();
		
	public:
		D3DXHANDLE m_t;
		D3DXHANDLE m_sceneTexture;
	};
	/*
	 *	
	 */
	class cPostEffectScaleOriShader : public cShader
	{
	public:
		cPostEffectScaleOriShader() {}
		virtual ~cPostEffectScaleOriShader() {}
		virtual void registHandle();
		
	public:
		D3DXHANDLE m_t;
		D3DXHANDLE m_sceneTexture;
	};
	/*
	 *	
	 */
	class cPostEffectScaleQuartShader : public cShader
	{
	public:
		cPostEffectScaleQuartShader() {}
		virtual ~cPostEffectScaleQuartShader() {}
		virtual void registHandle();
		
	public:
		D3DXHANDLE m_t;
		D3DXHANDLE m_sceneTexture;
	};
	/*
	 *	
	 */
	class cPostEffectBrightPassShader : public cShader
	{
	public:
		cPostEffectBrightPassShader() {}
		virtual ~cPostEffectBrightPassShader() {}
		virtual void registHandle();
		
	public:
		D3DXHANDLE m_t;
		D3DXHANDLE m_sceneTexture;
	};
	/*
	 *	
	 */
	class cPostEffectToneMapShader : public cShader
	{
	public:
		cPostEffectToneMapShader() {}
		virtual ~cPostEffectToneMapShader() {}
		virtual void registHandle();
		
	public:
		D3DXHANDLE m_t;
		D3DXHANDLE m_sceneTexture;
	};
	/*
	 *	
	 */
	class cPostEffectEdgeDetectShader : public cShader
	{
	public:
		cPostEffectEdgeDetectShader() {}
		virtual ~cPostEffectEdgeDetectShader() {}
		virtual void registHandle();
		
	public:
		D3DXHANDLE m_t;
		D3DXHANDLE m_sceneTexture;
		D3DXHANDLE m_texSampling;
	};
	/*
	 *	
	 */
	class cPostEffectNormalShader : public cShader
	{
	public:
		cPostEffectNormalShader() {}
		virtual ~cPostEffectNormalShader() {}
		virtual void setMeshParam(cMesh const* mesh, cEntityModel const* entity);
		virtual void registHandle();

	public:
		D3DXHANDLE m_t;
		D3DXHANDLE m_matWVP;
		D3DXHANDLE m_matWV;
	};
	/*
	 *	
	 */
	class cPostEffectNormalEdgeDetectShader : public cShader
	{
	public:
		cPostEffectNormalEdgeDetectShader() {}
		virtual ~cPostEffectNormalEdgeDetectShader() {}
		virtual void registHandle();
		
	public:
		D3DXHANDLE m_t;
		D3DXHANDLE m_sceneTexture;
		D3DXHANDLE m_texSampling;
	};
	/*
	 *	
	 */
	class cPostEffectDOFShader : public cShader
	{
	public:
		cPostEffectDOFShader() {}
		virtual ~cPostEffectDOFShader() {}
		virtual void setMeshParam(cMesh const* mesh, cEntityModel const* entity);
		virtual void registHandle();
		
	public:
		D3DXHANDLE m_t;
		D3DXHANDLE m_sceneTexture;
		D3DXHANDLE m_depthTexture;
		D3DXHANDLE m_oriSceneTexture;
		D3DXHANDLE m_focusDistance;
		D3DXHANDLE m_focusRange;
		D3DXHANDLE m_farClip;
	};
	/*
	*/
	class cPostEffectSSAOShader : public cShader
	{
	public:
		cPostEffectSSAOShader() {}
		virtual ~cPostEffectSSAOShader() {}
		virtual void registHandle();

	public:
		D3DXHANDLE m_t;
		D3DXHANDLE m_sceneTexture;
		D3DXHANDLE m_ssaoTexture;
		D3DXHANDLE m_textureSize;
	};
	/*
	*/
	class cPostEffectLightScatteringShader : public cShader
	{
	public:
		cPostEffectLightScatteringShader() {}
		virtual ~cPostEffectLightScatteringShader() {}
		virtual void registHandle();

	public:
		D3DXHANDLE m_t;
		D3DXHANDLE m_screenLightPos;
		D3DXHANDLE m_density;
		D3DXHANDLE m_weight;
		D3DXHANDLE m_exposure;
		D3DXHANDLE m_decay;
		D3DXHANDLE m_sceneTexture;
		D3DXHANDLE m_samplingTexture;
	};
	/*
	*/
	class cPostEffectDecalShader : public cShader
	{
	public:
		cPostEffectDecalShader() {}
		virtual ~cPostEffectDecalShader() {}
		virtual void registHandle();

	public:
		D3DXHANDLE m_t;
		D3DXHANDLE m_sceneTexture;
	};
}



#endif