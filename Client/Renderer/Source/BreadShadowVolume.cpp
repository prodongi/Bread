
#include "BreadShadowVolume.h"
#include "BreadLightManager.h"
#include "BreadCameraManager.h"
#include "BreadUberShaderManager.h"
#include "BreadEntityManager.h"
#include "BreadSceneManager.h"
#include "BreadGui.h"
#include "BreadCore.h"


namespace Bread
{
	#define EXTRUDE_EPSILON 0.1f

	/*
	*/
	cShadowBase* createShadowVolume()
	{
		return new cShadowVolume;
	}
	/*
	*	
	*/
	cShadowVolume::cShadowVolume()
	{
	}
	/*
	*	
	*/
	cShadowVolume::~cShadowVolume()
	{
	}
	/*
	*	
	*/
	bool cShadowVolume::initialize(uint width, uint height)
	{
		cShadowBase::initialize(width, height);
		initLight();
		initShader();

		return true;
	}
	/*
	*	
	*/
	void cShadowVolume::initLight()
	{
		cLight* light;
		light = _getLightMgr()->create();
		light->setType(D3DLIGHT_DIRECTIONAL);
		light->setColor(sVector3(0.0f, 1.0f, 0.0f));
		light->setPosition(sVector3(-3.0f, 3.0f, -4.0f));
		light->setDirection(sVector3(-1.0f, -1.0f, -1.0f));
		light->setEntity(_T("camtarget"));
		_getLightMgr()->pushBack(&light);
	}
	/*
	*	
	*/
	void cShadowVolume::initShader()
	{
		/*
		sVector4 ambient(1.0f, 1.0f, 1.0f, 1.0f);
		cUberShader* shader = _getUberShaderMgr()->get(_T("shadowvolume.fx"));
		shader->setFloat(shader->m_farClip, 20.0f - EXTRUDE_EPSILON);
		shader->setVector(shader->m_ambient, &ambient);
		shader->setUserData((void*)this);
		*/
	}
	/*
	*	
	*/
	void cShadowVolume::update(float /*elapsedtime*/)
	{
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
		light->setPosition(sVector3(pos.x+offsetV.x, pos.y+offsetV.y, pos.z+offsetV.z));
		entity->setPosition(light->getPosition());
	}
	/*
	*	
	*/
	void cShadowVolume::render()
	{
		renderScene();
		renderShadow();
	}
	/*
	*	
	*/
	void cShadowVolume::renderScene()
	{
		m_isScene = true;

		//_getSceneMgr()->renderSingleShader(_T("shadowvolume.fx"), _T("RenderSceneAmbient"));

	}
	/*
	*	
	*/
	void cShadowVolume::renderShadow()
	{
		m_isScene = false;

		cD3DSystem::getD3DDevice()->Clear(0, NULL, D3DCLEAR_STENCIL, D3DCOLOR_ARGB( 0, 170, 170, 170 ), 1.0f, 0);

		//cUberShader* shader = _getUberShaderMgr()->get(_T("shadowvolume.fx"));
		//cCamera* cam = _getCameraMgr()->getMain();
		cLight* light = _getLightMgr()->get(0);

		// light
		sVector4 lightPos, lightColor;
		light->getColor().to4(lightColor);
		light->getPosition().to4(lightPos);
		lightPos = sVector4(100.0f, 100.0f, 100.0f, 1.0f);
		/*
		shader->setVector(shader->m_lightPos, &lightPos);
		shader->setVector(shader->m_lightColor, &light->getColor());
		////
		shader->setMatrix(shader->m_proj, &cam->getProj());
		*/

		sCheckDevice const* checkDevice = _getD3DSystem()->getCheckDevice();
		TCHAR* shaderBuid;
		if (checkDevice->m_stencil2Side)	shaderBuid = _T("RenderShadowVolume2Sided");
		else								shaderBuid = _T("RenderShadowVolume");
		//shaderBuid = _T("ShowShadowVolume2Sided");

		//
		//sVector4 shadowColor(0.0f, 1.0f, 0.0f, 0.2f);
		//shader->setVector(shader->m_shadowColor, &shadowColor);
		//
		/*
		cEntityModel* entity = _getEntityMgr()->get(_T("0"));
		_getSceneMgr()->renderEntitySingleShader(_T("shadowvolume.fx"), shaderBuid, entity->getRenderDataBuid());
		_getSceneMgr()->renderEntitySingleShader(_T("shadowvolume.fx"), _T("RenderScene"), entity->getRenderDataBuid());
		*/

		_getSceneMgr()->renderSingleUberShader(_T("shadowvolume.fx"), shaderBuid);
		_getSceneMgr()->renderSingleUberShader(_T("shadowvolume.fx"), _T("RenderScene"));

	}
}

