
#include "stdafx.h"
#include "Test.h"
#include "BreadSceneManager.h"
#include "BreadEntityPlane.h"

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

	/// water
	cEntityPlane* plane = dynamicCast<cEntityPlane*>(_getSceneMgr()->createEntity<cEntityPlane>(_T("plane")));
	if (plane)
	{
		plane->setPosition(sVector3(0.0f, 10.0f, 0.0f));
		plane->createMesh(1000, 1000, 5, 5, false, _T("waterUberShader"), _T("base"), _T("grid.tga"), true);
	}

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