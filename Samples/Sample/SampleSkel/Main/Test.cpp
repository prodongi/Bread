
#include "stdafx.h"
#include "Test.h"
#include "BreadSceneManager.h"

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
	_getSceneMgr()->createScene(_T("plane.bsd"), 0);

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
}