
#include "stdafx.h"
#include "BreadPostProcessManager.h"
#include "BreadExceptionHandler.h"
#include "BreadCameraManager.h"
#include "BreadTextureManager.h"
#include "BreadSceneManager.h"
#include "BreadLightManager.h"
#include "BreadAnimation.h"
#include "BreadRenderer.h"
#include "BreadMaterial.h"
#include "BreadMaterial.h"
#include "BreadTerrain.h"
#include "MsgInfoList.h"
#include "TreasureApp.h"
#include "BreadCamera.h"
#include "BreadMesh.h"
#include "BreadPick.h"
#include "BreadGui.h"
#include "Game.h"

implementInstance(cTreasureApplication)

/*
*/
cTreasureApplication::cTreasureApplication() : m_camAngle(0.0f),
											   m_game(NULL)
{
	assignInstance(this)
}
/*
*/
cTreasureApplication::~cTreasureApplication()
{
	assignInstance(NULL)

	if (m_game)
	{
		m_game->finalize();
		safeDelete(m_game);
	}
}
/*
*	
*/
bool cTreasureApplication::initApp()
{
	if (!cApplication::initApp())
		return false;

	initCamera();
	if (!loadFont()) return false;
	if (!initUi())	return false;
	initLight();
	createShader();
	initPostProcess();
	initDebugFlag();
	if (!initGame()) return false;
	initState();
	
	return true;
}
/*
 *	
 */
void cTreasureApplication::initException()
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
void cTreasureApplication::initCamera()
{
	// add camera
	cCamera* camera = new cCamera(_T("main"));
	camera->setType(cCamera::TYPE_BACK);
	camera->setTargetHeight(0.0f);
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
bool cTreasureApplication::initUi()
{
	_getGui()->initConsole();

	return true;
}
/*
 *	
 */
void cTreasureApplication::initLight()
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
void cTreasureApplication::createShader()
{
}
/*
 *	
 */
void cTreasureApplication::initPostProcess()
{
	_getPostProcessMgr()->isHelperRender(cPostProcessManager::HELPER_DEPTH, true);
	_getPostProcessMgr()->addActive(cPostProcessManager::DECAL);
}
/*
 *	
 */
bool cTreasureApplication::initGame()
{
	assert(!m_game);
	m_game = new cGame;
	return m_game->initialize();
}
/*
*/
bool cTreasureApplication::loadFont()
{
	if (!_getFontMgr()->loadFreeTypeFont(_T("font\\comic.ttf")))
		return false;
	if (!_getFontMgr()->loadFreeTypeFont(_T("font\\arial.ttf")))
		return false;
	if (!_getFontMgr()->loadFreeTypeFont(_T("font\\times.ttf")))
		return false;
	if (!_getFontMgr()->loadFreeTypeFont(_T("font\\tahoma.ttf")))
		return false;

	_getFontMgr()->setDefaultFont(_T("comic"));

	return true;
}
/*
 *	
 */
void cTreasureApplication::initDebugFlag()
{
	_setIsRender(RENDER_MESH_CULLFIGURE, false);
	_setIsRender(RENDER_BONE, false);
	_setIsRender(RENDER_GRID, false);
	_setIsRender(RENDER_SCENE, true);
	_setIsRender(RENDER_SCENE_OCTREE, false);
	_setIsRenderValue1(RENDER_SCENE_OCTREE, 4);
	_setIsRender(RENDER_TERRAIN, false);
	_setIsRender(RENDER_PICK, true);
	_setIsRender(RENDER_WIRE, false);
	_setIsRender(RENDER_NORMAL, false);
	_setIsRender(RENDER_TANGENT, false);
	_setIsRender(RENDER_BINORMAL, false);
	_setIsRender(RENDER_RENDERLIST_CULLFIGURE, true);
	_setIsRender(RENDER_PROFILE, false);
	_setIsRenderValue1(RENDER_PROFILE, 800);
	_setIsRenderValue2(RENDER_PROFILE, 30);
	_setIsRender(RENDER_DEBUG_INFO, true);
	_setIsRender(RENDER_FPS, true);
	_setIsRender(RENDER_MOVE_PROXY_PATH, true);
}
/*
*/
void cTreasureApplication::initState()
{
	sMsgGameState msg;
	msg.m_cmd = "set";
	msg.m_state = "login";
	_getMsgMgr()->execute(msg);

	msg.m_cmd = "change";
	msg.m_state = "play";
	msg.m_delay = 2.0f;
	_getMsgMgr()->execute(msg);
}
/*
*/
void cTreasureApplication::mouseEventCallBack()
{
	m_game->mouseEventCallBack();
}
/*
*	
*/
void cTreasureApplication::update(float elapsedtime)
{
	cApplication::update(elapsedtime);
	m_game->update(elapsedtime);

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
*/
void cTreasureApplication::render()
{
	cApplication::render();
}
/*
 *	
 */
int cTreasureApplication::renderDebugInfo(int y)
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

	y = m_game->renderDebugInfo(y);

	return y;
}
/*
 *	
 */
void cTreasureApplication::beforeRender3D()	{	m_game->beforeRender3D();	}
void cTreasureApplication::afterRender3D()	{	m_game->afterRender3D();		}
void cTreasureApplication::beforeRender2D()	{	m_game->beforeRender2D();	}
void cTreasureApplication::afterRender2D()	{	m_game->afterRender2D();		}














