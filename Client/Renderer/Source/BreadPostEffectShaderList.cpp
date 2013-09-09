
#include "BreadPostEffectShaderList.h"
#include "BreadMaterial.h"
#include "BreadEntityModel.h"
#include "BreadCameraManager.h"

namespace Bread
{
	static sMatrix4 m;
	static sMatrix4 matWorld;
	static sMatrix4 matPos;
	static sMatrix4 matWVP;
	static sMatrix4 matWV;
	static sMatrix4 matWVPT;
	static sMatrix4 matWLP;

	/*
	 *	
	 */
	void cPostEffectMonoChromeShader::registHandle()
	{
		m_t = insertTechnique(_T("T"));
		m_luminance = insertParameter(_T("luminance"));
		m_sceneTexture = insertParameter(_T("sceneTexture"));
	}

	/*
	 *	
	 */
	void cPostEffectSepiaShader::registHandle()
	{
		m_t = insertTechnique(_T("T"));
		m_intensityConverter = insertParameter(_T("intensityConverter"));
		m_sepiaConvert = insertParameter(_T("sepiaConvert"));
		m_sceneTexture = insertParameter(_T("sceneTexture"));
	}

	/*
	 *	
	 */
	void cPostEffectInverseShader::registHandle()
	{
		m_t = insertTechnique(_T("T"));
		m_sceneTexture = insertParameter(_T("sceneTexture"));
	}

	/*
	 *	
	 */
	void cPostEffectGBlurVShader::registHandle()
	{
		m_t = insertTechnique(_T("T"));
		m_sceneTexture = insertParameter(_T("sceneTexture"));
		m_samplingV = insertParameter(_T("samplingV"));
	}

	/*
	 *	
	 */
	void cPostEffectGBlurHShader::registHandle()
	{
		m_t = insertTechnique(_T("T"));
		m_sceneTexture = insertParameter(_T("sceneTexture"));
		m_samplingU = insertParameter(_T("samplingU"));
	}

	/*
	 *	
	 */
	void cPostEffectBloomVShader::registHandle()
	{
		m_t = insertTechnique(_T("T"));
		m_sceneTexture = insertParameter(_T("sceneTexture"));
		m_samplingV = insertParameter(_T("samplingV"));
	}

	/*
	 *	
	 */
	void cPostEffectBloomHShader::registHandle()
	{
		m_t = insertTechnique(_T("T"));
		m_sceneTexture = insertParameter(_T("sceneTexture"));
		m_samplingU = insertParameter(_T("samplingU"));
	}

	/*
	 *	
	 */
	void cPostEffectScaleOriShader::registHandle()
	{
		m_t = insertTechnique(_T("T"));
		m_sceneTexture = insertParameter(_T("sceneTexture"));
	}

	/*
	 *	
	 */
	void cPostEffectScaleHalfShader::registHandle()
	{
		m_t = insertTechnique(_T("T"));
		m_sceneTexture = insertParameter(_T("sceneTexture"));
	}

	/*
	 *	
	 */
	void cPostEffectScaleQuartShader::registHandle()
	{
		m_t = insertTechnique(_T("T"));
		m_sceneTexture = insertParameter(_T("sceneTexture"));
	}

	/*
	 *	
	 */
	void cPostEffectBrightPassShader::registHandle()
	{
		m_t = insertTechnique(_T("T"));
		m_sceneTexture = insertParameter(_T("sceneTexture"));
	}

	/*
	 *	
	 */
	void cPostEffectToneMapShader::registHandle()
	{
		m_t = insertTechnique(_T("T"));
		m_sceneTexture = insertParameter(_T("sceneTexture"));
	}

	/*
	 *	
	 */
	void cPostEffectEdgeDetectShader::registHandle()
	{
		m_t = insertTechnique(_T("T"));
		m_sceneTexture = insertParameter(_T("sceneTexture"));
		m_texSampling = insertParameter(_T("texelSampling"));
	}

	/*
	*	
	*/
	void cPostEffectNormalShader::setMeshParam(cMesh const* mesh, cEntityModel const* entity)
	{
		unreferencedParameter(mesh);

		cCamera* cam = _getCameraMgr()->getMain();

		// world
		entity->calcuMatWorld(matWorld);
		mulMat4(matWorld, cam->getView(), matWV);
		mulMat4(matWV, cam->getProj(), matWVP);

		setMatrix(m_matWVP, &matWVP);
		setMatrix(m_matWV, &matWV);
	}
	/*
	*	
	*/
	void cPostEffectNormalShader::registHandle()
	{
		m_t = insertTechnique(_T("T"));
		m_matWVP = insertParameter(_T("matWVP"));
		m_matWV = insertParameter(_T("matWV"));
	}

	/*
	 *	
	 */
	void cPostEffectNormalEdgeDetectShader::registHandle()
	{
		m_t = insertTechnique(_T("T"));
		m_sceneTexture = insertParameter(_T("normalTexture"));
		m_texSampling = insertParameter(_T("texelSampling"));
	}

	/*
	*/
	void cPostEffectDOFShader::registHandle()
	{
		m_t = insertTechnique(_T("T"));
		m_sceneTexture = insertParameter(_T("blurTexture"));
		m_depthTexture = insertParameter(_T("depthTexture"));
		m_oriSceneTexture = insertParameter(_T("oriSceneTexture"));
		m_focusDistance = insertParameter(_T("focusDistance"));
		m_focusRange = insertParameter(_T("focusRange"));
		m_farClip = insertParameter(_T("farClip"));
	}
	/*
	*/
	void cPostEffectDOFShader::setMeshParam(cMesh const* /*mesh*/, cEntityModel const* /*entity*/)
	{
	}

	/*
	*/
	void cPostEffectSSAOShader::registHandle()
	{
		m_t = insertTechnique(_T("T"));
		m_sceneTexture = insertParameter(_T("sceneTexture"));
		m_ssaoTexture = insertParameter(_T("ssaoTexture"));
		m_textureSize = insertParameter(_T("textureSize"));
	}

	/*
	*/
	void cPostEffectLightScatteringShader::registHandle()
	{
		m_t = insertTechnique(_T("T"));
		m_screenLightPos = insertParameter(_T("screenLightPos"));
		m_density = insertParameter(_T("density"));
		m_weight = insertParameter(_T("weight"));
		m_exposure = insertParameter(_T("exposure"));
		m_decay = insertParameter(_T("decay"));
		m_sceneTexture = insertParameter(_T("sceneTexture"));
		m_samplingTexture = insertParameter(_T("samplingTexture"));
	}

	/*
	*/
	void cPostEffectDecalShader::registHandle()
	{
		m_t = insertTechnique(_T("T"));
		m_sceneTexture = insertParameter(_T("sceneTexture"));
	}
}