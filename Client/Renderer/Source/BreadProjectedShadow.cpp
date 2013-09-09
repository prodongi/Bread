
#include "BreadProjectedShadow.h"
#include "BreadLightManager.h"
#include "BreadRenderPropertyManager.h"
#include "BreadEntityManager.h"
#include "BreadSceneManager.h"
#include "BreadCameraManager.h"
#include "BreadGui.h"
#include "BreadMaterial.h"
#include "BreadCore.h"
#include "BreadShaderList.h"
#include "BreadDebugRenderManager.h"
#include "BreadBatchRenderManager.h"

namespace Bread
{
	/*
	*/
	cShadowBase* createShadowProjected()
	{
		return new cProjectedShadow;
	}
	/*
	 *	
	 */
	cProjectedShadow::cProjectedShadow() : m_rttShadow(NULL), 
										   m_shader(NULL)
	{
	}
	/*
	 *	
	 */
	cProjectedShadow::~cProjectedShadow() 
	{ 
		safeDelete(m_rttShadow);
		safeDelete(m_shader);
	}
	/*
	*	
	*/
	bool cProjectedShadow::initialize(uint width, uint height)
	{
		cShadowBase::initialize(width, height);

		initRtt();
		initMatrix();
		if (!initShader())
			return false;

		return true;
	}
	/*
	*	
	*/
	void cProjectedShadow::initRtt()
	{
		m_rttShadow = new cRenderToTextureNAA;
		m_rttShadow->initialize(0, m_width, m_height);
	}
	/*
	*	
	*/
	void cProjectedShadow::initMatrix()
	{
		updateMatrix();
		// tex
		m_matShadowTex = sMatrix4(0.5f, 0.0f, 0.0f, 0.0f,
									   0.0f,-0.5f, 0.0f, 0.0f,
									   0.0f, 0.0f, 1.0f, 0.0f,
									   0.5f, 0.5f, 0.0f, 1.0f);
	}
	/*
	 *	
	 */
	bool cProjectedShadow::initShader()
	{
		assert(!m_shader);
		m_shader = new cProjectedShadowShader;
		if (!m_shader->createEffect(_T("projectedshadow.fx"), cD3DSystem::getD3DDevice(), cShader::makeFlags()))
			return false;
		m_shader->setUserData(this);
		return true;
	}
	/*
	*	
	*/
	void cProjectedShadow::update(float elapsedtime)
	{
		unreferencedParameter(elapsedtime);
/*
		cLight* light = _getLightMgr()->get(0);
		sVector3 pos = light->getPosition();
		cCamera* cam = _getCameraMgr()->getMain();

		sVector3 offsetV(0.0f, 0.0f, 0.0f);
		if (_getInputSystem()->isPressedKey(DIK_T))
			cam->calculMoveOffset(cCamera::MOVE_FORWARD, 5, offsetV);
		if (_getInputSystem()->isPressedKey(DIK_G))
			cam->calculMoveOffset(cCamera::MOVE_BACKWARD, 5, offsetV);
		if (_getInputSystem()->isPressedKey(DIK_F))
			cam->calculMoveOffset(cCamera::MOVE_LEFTSTEP, 5, offsetV);
		if (_getInputSystem()->isPressedKey(DIK_H))
			cam->calculMoveOffset(cCamera::MOVE_RIGHTSTEP, 5, offsetV);

		cEntityModel* entity = _getEntityMgr()->get(_T("camtarget"));
		light->setPosition(sVector3(pos.m_x+offsetV.x, pos.m_y+offsetV.y, pos.m_z+offsetV.z));
		entity->setPosition(light->getPosition());

		cUberShader* shader = _getUberShaderMgr()->get(_T("projectedshadow.fx"));
		shader->setVector(_T("vLightPos"), &light->getPosition());
		shader->setTexture(_T("ShadowMap"), m_rttShadow->getTexture());
*/
		updateMatrix();
	}
	/*
	 *	
	 */
	void cProjectedShadow::updateMatrix()
	{
		cLight* light = _getLightMgr()->get(0);
		if (light)
		{
			// view
			sVector3 eye, at, up;
			eye = light->getPosition();
			at = sVector3(0.0f, 0.0f, 0.0f);
			up = sVector3(0.0f, 1.0f, 0.0f);
			m_matShadowView.makeViewMatrix(eye, at);
			// projection
			m_matShadowProj.makePerspectiveMatrix(D3DX_PI/2.5f, 1.0f, 0.1f, 10000.0f);
			//
			mulMat4(m_matShadowView, m_matShadowProj, m_matShadowVP);
		}
	}
	/*
	*	
	*/
	void cProjectedShadow::makeShadowMap()
	{
		//m_rttShadow->begin(true, 0xffffffff);
		_getRenderPropertyMgr()->begin();
		_getRenderPropertyMgr()->setProperty(sRenderProperty::COLOROP, D3DTOP_SELECTARG1);
		_getRenderPropertyMgr()->setProperty(sRenderProperty::COLORARG1, D3DTA_DIFFUSE);

		//_getSceneMgr()->renderSingleUberShader(_T("projectedshadow.fx"), _T("TProjShadow"), cRenderList::MASK_NOT_SCENE);

		cRenderer::renderSingleScene(m_rttShadow, 1, m_shader, _T("TProjShadow"), true, cBatchRenderManager::MASK_NOT_SCENE, 0xffffffff);

		_getRenderPropertyMgr()->end();
		//m_rttShadow->end();
	}
	/*
	*	
	*/
	void cProjectedShadow::renderDebug()
	{
		_getDebugRenderMgr()->addScreen(800.0f, 0.0f, 128.0f, 128.0f, m_rttShadow->getTexture());
	}
	/*
	*	
	*/
	void cProjectedShadow::beforeRender3D()
	{
		makeShadowMap();
	}
	/*
	*	
	*/
	void cProjectedShadow::afterRender2D()
	{
		renderDebug();
	}
}
















