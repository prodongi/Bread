
#include "stdafx.h"
#include "Test.h"
#include "BreadSceneManager.h"
#include "BreadFileHelper.h"
#include "BreadTextureManager.h"

/*
*/
sTextureCache::~sTextureCache()
{
	m_fileList.clear();
}
/*
*/
void sTextureCache::init()
{
	m_update.begin(0.1f, true);
	cFileHelper::getFileList(_T("ScreenShot"), _T("jpg"), m_fileList);
}
/*
*/
void sTextureCache::update(float elapsedtime)
{
	if (!m_update.m_active)
		return ;
	if (m_fileList.empty())
	{
		m_update.end();
		return ;
	}
	if (!m_update.update(elapsedtime))
	{
		std_string fname = m_fileList.back();
		_getTextureMgr()->loadTexture(fname.c_str());
		m_fileList.pop_back();
	}
}

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

	m_textureCache.init();

	return true;
}
/*
 *	
 */
void cTest::update(float elapsedtime)
{
	updateMouse();
	updateKeyboard();

	m_textureCache.update(elapsedtime);
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