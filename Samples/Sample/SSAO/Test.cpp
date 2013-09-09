
#include "stdafx.h"
#include "Test.h"
#include "BreadSceneManager.h"
#include "BreadDebugRenderManager.h"
#include "BreadPostProcessManager.h"
#include "BreadTextureManager.h"


/*
 *	
 */
cTest::cTest()
{
}
/*
 *	
 */
cTest::~cTest()
{
}
/*
 *	
 */
bool cTest::initialize()
{
	if (!initEntity())
		return false;
	
	return true;
}
/*
*/
bool cTest::initEntity()
{
	_getSceneMgr()->createScene(_T("human.bsd"), 0);

	return true;
}
/*
 *	
 */
void cTest::update(float elapsedtime)
{
	unreferencedParameter(elapsedtime);

	updateMouse();
	updateKeyboard();
}
/*
*/
void cTest::updateMouse()
{
}
/*
*/
void cTest::updateKeyboard()
{
}
/*
 *	
 */
int cTest::renderDebugInfo(int y)
{
	return y;
}
/*
 *	
 */
void cTest::beforeRender3D()
{
}
/*
 *	
 */
void cTest::afterRender3D()
{
}
/*
 *	
 */
void cTest::beforeRender2D()
{
}
/*
 *	
 */
void cTest::afterRender2D()
{
	_getDebugRenderMgr()->addScreen(140.0f, 550.0f, 128.0f, 128.0f, _getPostProcessMgr()->getHelperTexture(cPostProcessManager::HELPER_DEPTH));
}