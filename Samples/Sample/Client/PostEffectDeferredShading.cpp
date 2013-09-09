
#include "stdafx.h"
#include "PostEffectDeferredShading.h"
#include "BreadTextureManager.h"
#include "BreadSceneManager.h"
#include "BreadCameraManager.h"
#include "BreadRenderPropertyManager.h"
#include "BreadLightManager.h"
#include "BreadGui.h"
#include "BreadUberShaderManager.h"
#include "BreadFrustum.h"

uint cDeferredLightManager::MAX_LIGHT = 8;
float cDeferredLightManager::MIN_SQRLEN = 1.0f;
float cDeferredLightManager::MAX_CAM_SQRLEN = 10.0f;

/*
 *	
 */
void cDeferredLightManager::add(cLight const* light)
{
	if (cDeferredLightManager::MAX_LIGHT <= size())
		return ;

	if (D3DLIGHT_DIRECTIONAL == light->getType())
	{
		m_list.push_back(light);
	}
	else if (D3DLIGHT_POINT == light->getType())
	{
		cCamera* cam = _getCameraMgr()->getMain();
		// check frustum
		if (!cam->getFrustum()->isVisible(light->getPosition(), light->getRange()))
			return ;

		// check from camera to light length
		sVector3 eyePos(cam->getEye());
		float sqrLen = eyePos.lengthSquare(light->getPosition());
		if (cDeferredLightManager::MAX_CAM_SQRLEN > sqrLen)
			return ;

		// check the other light distance
		std::vector<cLight const*>::iterator it = m_list.begin();
		for (; it != m_list.end(); ++it)
		{
			sqrLen = (*it)->getPosition().lengthSquare(light->getPosition());
			if (sqrLen < cDeferredLightManager::MIN_SQRLEN)
				return ;
		}
		m_list.push_back(light);
	}
}

/*
 *	
 */
cPostEffectDeferredShading::cPostEffectDeferredShading() : m_rttMaterial(NULL), 
														   m_rttNormal(NULL),
														   m_rttXY(NULL), 
														   m_rttZ(NULL),
														   m_lightRotV(0.0f)
{
}
/*
 *	
 */
cPostEffectDeferredShading::~cPostEffectDeferredShading()
{
	safeDelete(m_rttMaterial);
	safeDelete(m_rttNormal);
	safeDelete(m_rttXY);
	safeDelete(m_rttZ);
}
/*
 *	
 */
bool cPostEffectDeferredShading::initialize(uint width, uint height)
{
	m_screen.set(0.0f, 0.0f, (float)width, (float)height, 0xffffffff);

	//
	m_rttMaterial = new cRenderToTextureNAA;
	m_rttMaterial->initialize(0, width, height);
	m_rttNormal = new cRenderToTextureNAA;
	m_rttNormal->initialize(1, width, height, false, D3DFMT_X8R8G8B8);
	m_rttXY = new cRenderToTextureNAA;
	m_rttXY->initialize(2, width, height, false, D3DFMT_G16R16F);
	m_rttZ = new cRenderToTextureNAA;
	m_rttZ->initialize(3, width, height, false, D3DFMT_G16R16F);

	initLight();

	return true;
}
/*
 *
 */
void cPostEffectDeferredShading::initLight()
{
	//
	cLight* light;

	light = _getLightMgr()->create();
	light->setType(D3DLIGHT_POINT);
	light->setColor(sVector3(0.0f, 1.0f, 0.0f));
	light->setPosition(sVector3(-3.0f, 1.0f, 4.0f));
	light->setRange(5.0f);
	light->setEntity(_T("camtarget"));
	_getLightMgr()->pushBack(&light);

	light = _getLightMgr()->create();
	light->setType((int)D3DLIGHT_DIRECTIONAL);
	light->setColor(sVector3(0.2f, 0.2f, 0.2f));
	light->setDirection(sVector3(0.7f, 1.0f, 0.5f));
	light->setPosition(sVector3(10.0f, 10.0f, 10.0f));
	light->calculMatVP();
	_getLightMgr()->pushBack(&light);
	
	for (int i = 0; i < 10; ++i)
	{
		light = _getLightMgr()->create();
		light->setType(D3DLIGHT_POINT);
		light->setColor(sVector3(0.1f * (float)i, 0.0f, 0.0f));
		light->setPosition(sVector3(1.0f+i, 1.0f+i, 1.0f+i));
		light->setRange(5.0f);
		_getLightMgr()->pushBack(&light);
	}
}
/*
 *	
 */
void cPostEffectDeferredShading::render(cTexture* /*originalTexture*/)
{
	setupMatrix();
	renderBuild();
	renderLight();
}
/*
 *	
 */
void cPostEffectDeferredShading::update(float elapsedtime)
{
	updateLight(elapsedtime);
	updateDefLightMgr();
}
/*
 *	
 */
void cPostEffectDeferredShading::updateLight(float elapsedtime)
{
	m_lightRotV = elapsedtime;

	float range = 0.0f;
	if (_getInputSystem()->isPressedKey(DIK_1))			range = 0.1f;
	else if (_getInputSystem()->isPressedKey(DIK_2))	range = -0.1f;

	D3DXVECTOR3 _lightPos;
	D3DXMATRIXA16 matRotY;
	D3DXMatrixRotationY(&matRotY, m_lightRotV);
	uint lightNum = _getLightMgr()->getBucketSize();
	for (uint i = 0; i < lightNum; ++i)
	{
		cLight* light = _getLightMgr()->get(i);
		if (D3DLIGHT_POINT == light->getType())
		{
			// modify range
			light->setRange(light->getRange() + range);
			// rotation position
			light->getPosition().to3(_lightPos);
			D3DXVec3TransformCoord(&_lightPos, &_lightPos, &matRotY);
			light->setPosition(sVector3(_lightPos.x, _lightPos.y, _lightPos.z));
		}
	}
}
/*
 *	
 */
void cPostEffectDeferredShading::updateDefLightMgr()
{
	m_defLightMgr.clear();
	uint lightNum = _getLightMgr()->getBucketSize();
	for (uint i = 0; i < lightNum; ++i)
	{
		m_defLightMgr.add(_getLightMgr()->get(i));
	}
}
/*
 *	
 */
void cPostEffectDeferredShading::setupMatrix()
{
	cCamera* cam = _getCameraMgr()->getMain();
	cUberShader* shader = _getUberShaderMgr()->get(_T("deferred.fx"));

	D3DXMatrixInverse(&m_matViewInv, NULL, &cam->getView());
	shader->setMatrix(_T("c_mViewInverse"), &m_matViewInv);
	shader->setMatrix(_T("c_mViewProjection"), &cam->getVP());
}
/*
 *	
 */
void cPostEffectDeferredShading::renderBuild()
{
	m_rttMaterial->begin(true);
	m_rttNormal->begin(true);
	m_rttXY->begin(true);
	m_rttZ->begin(true);

	_getSceneMgr()->renderSingleUberShader(_T("deferred.fx"), _T("TBuild"));

	m_rttMaterial->end();
	m_rttNormal->end();
	m_rttXY->end();
	m_rttZ->end();
}
/*
 *	
 */
void cPostEffectDeferredShading::renderLight()
{
	_getRenderPropertyMgr()->begin();
	_getRenderPropertyMgr()->setProperty(sRenderProperty::ZENABLE, FALSE);
	_getRenderPropertyMgr()->setProperty(sRenderProperty::CULLMODE, D3DCULL_CCW);
	_getRenderPropertyMgr()->setProperty(sRenderProperty::ALPHABLENDENABLE, TRUE);
	_getRenderPropertyMgr()->setProperty(sRenderProperty::SRCBLEND, D3DBLEND_ONE);
	_getRenderPropertyMgr()->setProperty(sRenderProperty::DESTBLEND, D3DBLEND_ONE);
	_getRenderPropertyMgr()->setProperty(sRenderProperty::SCISSORTESTENABLE, TRUE);

	RECT rect;
	cUberShader* shader = _getUberShaderMgr()->get(_T("deferred.fx"));
	shader->setTexture(_T("c_tSceneMaterialMap"), m_rttMaterial->getTexture());
	shader->setTexture(_T("c_tSceneNormalMap"), m_rttNormal->getTexture());
	shader->setTexture(_T("c_tScenePositionXYMap"), m_rttXY->getTexture());
	shader->setTexture(_T("c_tScenePositionZMap"), m_rttZ->getTexture());

	uint lightNum = m_defLightMgr.size();
	for (uint i = 0; i < lightNum; ++i)
	{
		cLight const* light = m_defLightMgr.get(i);

		light->determineScissorRect(&rect);
		cD3DSystem::getD3DDevice()->SetScissorRect(&rect);

		if (D3DLIGHT_DIRECTIONAL == light->getType())
			shader->setTechnique(_T("TDirectionalLight"));
		else if (D3DLIGHT_POINT == light->getType())
		{
			shader->setTechnique(_T("TPointLight"));
			shader->setFloat(_T("range"), light->getRange());
		}

		shader->setVector(_T("c_vLightDir"), &light->getDirection());
		shader->setVector(_T("c_vLightPos"), &light->getPosition());
		shader->setVector(_T("c_vLightDiffuse"), &light->getColor());
		shader->setVector(_T("c_vLightSpecular"), &light->getColor());
		shader->setFloat(_T("c_fSpecularPower"), 8.0f);

		cRenderer::renderScreen(shader, &m_screen);
	}
	_getRenderPropertyMgr()->end();
}
/*
 *	
 */
int cPostEffectDeferredShading::renderDebugInfo(int y)
{
	y += 16;
	TCHAR str[NAME_LEN];

	_getFontMgr()->drawD3DText(10, y+=16, _T("[Deferred Shading]"), cD3DFont::HEIGHT_14, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	cLight* light = _getLightMgr()->get(0);
	if (light)
	{
		_stprintf_s(str, NAME_LEN, _T("light0 range: %f"), light->getRange());
		_getFontMgr()->drawD3DText(10, y+=16, str);
	}

	_stprintf_s(str, NAME_LEN, _T("cam num: %d"), m_defLightMgr.size());
	_getFontMgr()->drawD3DText(10, y+=16, str);

	return y;
}















