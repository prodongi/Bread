
#include "stdafx.h"
#include "SampleApp.h"
#include "BreadRenderer.h"
#include "BreadGui.h"
#include "BreadCameraManager.h"
#include "BreadCamera.h"
#include "BreadMaterial.h"
#include "BreadSceneManager.h"
#include "BreadTextureManager.h"
#include "BreadMaterial.h"
#include "BreadMesh.h"
#include "Shader.h"
#include "BreadPick.h"
#include "EntityAvatar.h"
#include "BreadLightManager.h"
#include "BreadExceptionHandler.h"
#include "BreadAnimation.h"
#include "BreadTerrain.h"
#include "BreadPostProcessManager.h"

implementInstance(cSampleApplication)

/*
*	
*/
bool cSampleApplication::initApp()
{
	if (!cApplication::initApp())
		return false;

	initCamera();
	if (!loadFont()) return false;
	if (!initUi())	return false;
	initLight();
	createShader();
	initPostProcess();
	if (!initTest()) return false;
	initDebugFlag();

	return true;
}
/*
 *	
 */
void cSampleApplication::initException()
{
	sExceptionMailInfo mailInfo;
	mailInfo.m_isSend = true;
	strcpy(mailInfo.m_recvAddress, "prodongi@hotmail.com");
	strcpy(mailInfo.m_recvName, "prodongi");
	cExceptionHandler::setMailInfo(mailInfo);
}
/*
 *	
 */
void cSampleApplication::initCamera()
{
	// add camera
	cCamera* camera = new cCamera(_T("main"));
	camera->setType(cCamera::TYPE_BACK);
//	camera->setTargetEntity(_T("0"));
	camera->setTargetHeight(0.0f);
//	camera->setAtEntity(_T("camtarget"));
	camera->set(sVector3(200.0f, 200.0f, 0.0f), sVector3(0.0f, 0.0f, 0.0f));
	camera->setMaxAtToEyeLength(10000.0f);
	camera->setMinAtToEyeLength(1.0f);
	_getCameraMgr()->add(camera);
	_getCameraMgr()->setMain(_T("main"));
	_getCameraMgr()->setActive(_T("main"));
}
/*
 *	
 */
bool cSampleApplication::initUi()
{
	/*
	if (!_getGui()->load(_T("ui\\exam.bi")))
		return false;
	*/
	_getGui()->initConsole();

	return true;
}
/*
 *	
 */
void cSampleApplication::initLight()
{
	sVector3 pos(200.0f, 200.0f, 0.0f);
	sVector3 at(0.0f, 0.0f, 0.0f);
	sVector3 dir;
	subVec3(at, pos, dir);
	dir.normalize();

	cLight* light = _getLightMgr()->create();
	light->setType(D3DLIGHT_DIRECTIONAL);
	light->setEntity(_T("lighttarget"));
	light->setColor(sVector3(0.0f, 1.0f, 0.0f));
	light->setPosition(pos);
	light->setDirection(dir);
	_getLightMgr()->pushBack(&light);
}
/*
 *	
 */
void cSampleApplication::createShader()
{
}
/*
 *	
 */
void cSampleApplication::initPostProcess()
{
	_getPostProcessMgr()->isHelperRender(cPostProcessManager::HELPER_DEPTH, true);

	_getPostProcessMgr()->addActive(cPostProcessManager::LIGHT_SCATTERING);
}
/*
 *	
 */
bool cSampleApplication::initTest()
{
	return m_test.initialize();
}
/*
*/
bool cSampleApplication::loadFont()
{
	if (!_getFontMgr()->loadFreeTypeFont(_T("font\\comic.ttf")))
		return false;
	if (!_getFontMgr()->loadFreeTypeFont(_T("font\\arial.ttf")))
		return false;
	if (!_getFontMgr()->loadFreeTypeFont(_T("font\\times.ttf")))
		return false;
	if (!_getFontMgr()->loadFreeTypeFont(_T("font\\tahoma.ttf")))
		return false;

	//cFreeTypeLine line;
	//_getFontMgr()->makeFreeTypeLine(sFreeTypeFontDesc(_T("comic"), 12), _T("aa"), line);
	_getFontMgr()->setDefaultFont(_T("comic"));

	return true;
}
/*
 *	
 */
void cSampleApplication::initDebugFlag()
{
	_setIsRender(RENDER_MESH_CULLFIGURE, false);
	_setIsRender(RENDER_BONE, true);
	_setIsRender(RENDER_GRID, false);
	_setIsRender(RENDER_SCENE, true);
	_setIsRender(RENDER_SCENE_OCTREE, false);
	_setIsRenderValue1(RENDER_SCENE_OCTREE, 1);
	_setIsRender(RENDER_TERRAIN, false);
	_setIsRender(RENDER_PICK, true);
	_setIsRender(RENDER_WIRE, false);
	_setIsRender(RENDER_NORMAL, false);
	_setIsRender(RENDER_TANGENT, false);
	_setIsRender(RENDER_BINORMAL, false);
	_setIsRender(RENDER_RENDERLIST_CULLFIGURE, false);
}
/*
*	
*/
void cSampleApplication::update(float elapsedtime)
{
	cApplication::update(elapsedtime);
	m_test.update(elapsedtime);

	float v = 0.1f;
	m_camAngle += v * elapsedtime;
	if (m_camAngle > cMath::TWOPI)
		m_camAngle = 0.0f;

	cLight* light = _getLightMgr()->get(0);
	if (light)
	{
		float r = light->getRange();
		if (_getInputSystem()->isPressedKey(DIK_O))
		{
			r += 0.1f;
			if (r >= 100.0f) r = 100.0f;
		}
		if (_getInputSystem()->isPressedKey(DIK_P))
		{
			r -= 0.1f;
			if (r < 0.1f)	r = 0.1f;
		}
		light->setRange(r);
	}
}
/*
 *	
 */
int cSampleApplication::renderDebugInfo(int y)
{
	y = cApplication::renderDebugInfo(10);
	y += 16;

	TCHAR str[128];
	_getFontMgr()->drawD3DText(10, y+=16, _T("[Sample]"), cD3DFont::HEIGHT_14, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	_stprintf(str, _T("cam angle: %.3f"), m_camAngle);
	_getFontMgr()->drawD3DText(10, y+=16, str);

	cLight* light = _getLightMgr()->get(0);
	if (light)
	{
		_stprintf(str, _T("light range: %.3f"), light->getRange());
		_getFontMgr()->drawD3DText(10, y+=16, str);
	}

	y = m_test.renderDebugInfo(y);

	return y;
}
/*
 *	
 */
void cSampleApplication::beforeRender3D()	{	m_test.beforeRender3D();	}
void cSampleApplication::afterRender3D()	{	m_test.afterRender3D();		}
void cSampleApplication::beforeRender2D()	{	m_test.beforeRender2D();	}
void cSampleApplication::afterRender2D()	{	m_test.afterRender2D();		}














