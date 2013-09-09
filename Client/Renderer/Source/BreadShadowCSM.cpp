
#include "BreadShadowCSM.h"
#include "BreadMath.h"
#include "BreadCameraManager.h"
#include "BreadLightManager.h"
#include "BreadVector2.h"
#include "BreadVector3.h"
#include "BreadMatrix4.h"
#include "BreadCore.h"
#include "BreadD3DDeclaration.h"
#include "BreadRenderPropertyManager.h"
#include "BreadSceneManager.h"
#include "BreadShaderList.h"
#include "BreadGui.h"
#include "BreadRenderToTexture.h"
#include "BreadDebugRenderManager.h"
#include "BreadTexture.h"
#include "BreadBatchRenderManager.h"

namespace Bread
{
	/*
	*/
	cShadowBase* createShadowCSM()
	{
		return new cShadowCSM;
	}

	/*
	*/
	bool cShadowCSM::sRenderContext::initialize(uint width, uint height)
	{
		assert(!m_rtt);
		m_rtt = new cRenderToTextureNAA;
		if (!m_rtt->initialize(0, width, height, true, D3DFMT_R32F))
			return false;

		return true;
	}

	/*
	*/
	cShadowCSM::cShadowCSM() : m_renderContext(NULL),
							   m_depthShader(NULL),
							   m_depthBias(0.0f)
	{
	}
	/*
	*/
	cShadowCSM::~cShadowCSM()
	{
		safeDeleteArray(m_renderContext);
		safeDelete(m_depthShader);
	}
	/*
	*/
	bool cShadowCSM::initialize(uint width, uint height)
	{
		if (!cShadowBase::initialize(width, height))
		{
			return false;
		}

		m_depthBias = 0.009f;
		m_lightDepthScale = 1.0f;
		m_fovYDiv = 1.0f;

		if (!initDepthShader())
			return false;
		if (!initRenderContext(_getEngineConfig()->getZNear(), _getEngineConfig()->getZFar()))
			return false;
		initMatTex();		

		return true;
	}
	/*
	*/
	bool cShadowCSM::initDepthShader()
	{
		assert(!m_depthShader);
		m_depthShader = new cLightDepthMapShader;
		if (!m_depthShader->createEffect(_T("lightdepthmap.fx"), cD3DSystem::getD3DDevice(), cShader::makeFlags()))
			return false;

		return true;
	}
	/*
	*/
	void cShadowCSM::initMatTex()
	{
		m_matTex = sMatrix4(0.5f, 0.0f, 0.0f, 0.0f,
							0.0f,-0.5f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							0.5f, 0.5f, 0.0f, 1.0f);
	}
	/*
	*/
	bool cShadowCSM::initRenderContext(float zNear, float zFar)
	{
		assert(!m_renderContext);
		m_renderContext = new sRenderContext[CASCADED_NUM];
		for (uint i = 0; i < CASCADED_NUM; ++i)
		{
			if (!m_renderContext[i].initialize(_getEngineConfig()->getShadowWidth(), _getEngineConfig()->getShadowHeight()))
				return false;
		}

		m_splitLogFactor = 0.947f;
		calcSplitDepths(zNear, zFar, m_splitLogFactor);
		
		return true;
	}
	/*
	*/
	void cShadowCSM::calcSplitDepths(float zNear, float zFar, float splitLogFactor)
	{
		float i_f = 1.0f;
		float cascadedCount = (float)(CASCADED_NUM);
		for (uint i = 0; i < CASCADED_NUM-1; ++i, i_f += 1.0f)
		{
			m_renderContext[i].m_depth = cMath::lerp(zNear + (i_f/cascadedCount) * (zFar - zNear),
													 zNear * powf(zFar/zNear, i_f/cascadedCount),
													 splitLogFactor);
		}
		m_renderContext[CASCADED_NUM-1].m_depth = zFar;
	}
	/*
	*/
	void cShadowCSM::update(float /*elapsedtime*/)
	{
		updateBiasByCamLength();
		updateInput();
		_getBatchRenderMgr()->extract(*this, cBatchRenderManager::MASK_ALL);//cRenderList::MASK_NOT_SCENE);
		updateViewPort();
	}
	/*
	*/
	void cShadowCSM::updateBiasByCamLength()
	{
		/// 다음 수치들은 테스트 해서 나온 값들이다.
		float minLength = 100.0f;
		float maxLength = 400.0f;
		float minBias = 0.001f;
		float maxBias = 0.009f;

		float camLength = _getCameraMgr()->getMain()->getAtToEyeLength();
		if (camLength < minLength)
		{
			m_depthBias = maxBias;
		}
		else if (camLength > maxLength)
		{
			m_depthBias = minBias;
		}
		else
		{
			float s = (camLength - minLength)/(maxLength - minLength);
			m_depthBias = cMath::lerp(maxBias, minBias, s);
		}
	}
	/*
	*/
	void cShadowCSM::updateInput()
	{
		if (_getInputSystem()->isPressedKey(DIK_N))
		{
			m_depthBias += 0.001f;
		}
		else if (_getInputSystem()->isPressedKey(DIK_M))
		{
			m_depthBias -= 0.001f;
		}

		if (_getInputSystem()->isPressedKey(DIK_H))
		{
			m_lightDepthScale += 0.001f;
		}
		else if (_getInputSystem()->isPressedKey(DIK_J))
		{
			m_lightDepthScale -= 0.001f;
			if (m_lightDepthScale < 0.01f)	m_lightDepthScale = 0.01f;
		}
		if (_getInputSystem()->isPressedKey(DIK_Y))
		{
			m_fovYDiv += 0.01f;
		}
		else if (_getInputSystem()->isPressedKey(DIK_U))
		{
			m_fovYDiv -= 0.01f;
			if (m_fovYDiv < 0.01f)	m_fovYDiv = 0.01f;
		}
		if (_getInputSystem()->isPressedKey(DIK_O))
		{
			m_splitLogFactor = max(0.0f, m_splitLogFactor - 0.001f);
			calcSplitDepths(_getEngineConfig()->getZNear(), _getEngineConfig()->getZFar(), m_splitLogFactor);
			trace(_T("splitLogFactor : %f\n"), m_splitLogFactor);
		}
		else if (_getInputSystem()->isPressedKey(DIK_P))
		{
			m_splitLogFactor = max(1.0f, m_splitLogFactor + 0.001f);

			calcSplitDepths(_getEngineConfig()->getZNear(), _getEngineConfig()->getZFar(), m_splitLogFactor);
			trace(_T("splitLogFactor : %f\n"), m_splitLogFactor);
		}
	}
	/*
	*/
	void cShadowCSM::updateViewPort()
	{
		for (uint i = 0; i < CASCADED_NUM; ++i)
		{
			calcRenderContextVP(i);
		}
	}
	/*
	*/
	void cShadowCSM::calcRenderContextVP(int contextIndex)
	{
		///
		float zNear = (0 == contextIndex) ? _getEngineConfig()->getZNear() : m_renderContext[contextIndex-1].m_depth;
		float zFar = m_renderContext[contextIndex].m_depth;

		cCamera* mainCam = _getCameraMgr()->getMain();
		cLight* mainLight = _getLightMgr()->get(0);

		/// get frustum box
		sBox frustumBox;
		mainCam->getFrustumBox(frustumBox, zNear, zFar, m_fovYDiv);
		sVector3* extents = frustumBox.m_v;

		/// transform frustum box into light world space
		sMatrix4 invMatLight;
		mainLight->getMatView().inverse(invMatLight);
		for (int i = 0; i < sBox::VERT_NUM; ++i)
		{
			mulMat4(invMatLight, extents[i], extents[i]);
		}

		/// calcul size
		frustumBox.align();
		float width = frustumBox.width();
		float height = frustumBox.height();
		float depth = frustumBox.depth();

		depth *= m_lightDepthScale;

		/// calcul camera pos
		sVector3 cameraPos;
		lerpVec3(frustumBox.minv(), frustumBox.maxv(), 0.5f, cameraPos);

		/// transform camerapos into camera world space
		mulMat4(mainLight->getMatView(), cameraPos, cameraPos);

		/// set new light pos
		sVector3 temp;
		float halfDepth = depth * 0.5f;
		mulVec3(mainLight->getDirection(), -halfDepth, temp);
		cameraPos += temp;

		sMatrix4 view, proj;
		view.makeViewMatrixDir(cameraPos, mainLight->getDirection());
		proj.makeOrthoMatrix(width, height, 0.0f, depth);

		mulMat4(view, proj, m_renderContext[contextIndex].m_vp);
		sVector3 eye(mainCam->getEye().x, mainCam->getEye().y, mainCam->getEye().z);
		m_renderContext[contextIndex].m_frustum.construct(view, proj, eye);
	}
	/*
	*/
	cTexture* cShadowCSM::getDepthTexture(uint cascadedIndex) const
	{
		return (cascadedIndex < CASCADED_NUM) ? m_renderContext[cascadedIndex].m_rtt->getTexture() : NULL;
	}
	/*
	*/
	void cShadowCSM::getCascadedZNear(sVector4& znear)
	{
		for (uint i = 0; i < CASCADED_NUM; ++i)
		{
			if (0 == i)
			{
				znear[i] = _getCameraMgr()->getMain()->getZNear();
			}
			else
			{
				znear[i] = m_renderContext[i-1].m_depth;
			}
		}
	}
	/*
	*/
	void cShadowCSM::beforeRender3D()
	{
		for (uint i = 0; i < CASCADED_NUM; ++i)
		{
			sRenderContext* info = m_renderContext + i;
			m_depthShader->setMatLP(info->m_vp);
			cRenderer::renderRenderListScene(info->m_rtt, 1, m_depthShader, _T("T"), true, info->m_renderList);
		}
	}
	/*
	*/
	void cShadowCSM::afterRender3D()
	{
	}
	/*
	*/
	void cShadowCSM::afterRender2D()
	{
		/*
		for (uint i = 0; i < CASCADED_NUM; ++i)
		{
			_getDebugRenderMgr()->addScreen(340.0f + 128.0f*i, 550.0f, 128.0f, 128.0f, m_renderContext[i].m_rtt->getTexture());
		}
		*/
	}
	/*
	*/
	int cShadowCSM::renderDebugInfo(int y)
	{
		TCHAR str[MAX_PATH];

		_stprintf(str, _T("depthBias: %f"), m_depthBias);
		_getFontMgr()->drawD3DText(600, y+=16, str);

		_stprintf(str, _T("lightDepthScale: %f"), m_lightDepthScale);
		_getFontMgr()->drawD3DText(600, y+=16, str);

		_stprintf(str, _T("fovYDiv: %f"), m_fovYDiv);
		_getFontMgr()->drawD3DText(600, y+=16, str);

		float camLength = _getCameraMgr()->getMain()->getAtToEyeLength();
		_stprintf(str, _T("camLength: %f"), camLength);
		_getFontMgr()->drawD3DText(600, y+=16, str);


		return y;
	}
}