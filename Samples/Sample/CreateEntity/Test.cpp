
#include "stdafx.h"
#include "Test.h"
#include "BreadSceneManager.h"
#include "BreadFileHelper.h"
#include "BreadPick.h"
#include "BreadGui.h"
#include "BreadAnimationActionEnum.h"

sModelList::~sModelList()
	{
		m_fileList.clear();
	}
	/*
	*/
	void sModelList::init()
	{
		cFileHelper::getFileList(_T("data\\character\\jang"), _T("bmd"), m_fileList, true);
	}
	/*
	*/
	std_string const& sModelList::getFileName(int index)
	{
		if (-1 == index)
			index = cMath::rand(m_fileList.size());
		return m_fileList[index];
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

	m_testModelList.init();
	m_mouseDown = false;

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
	if (_getInputSystem()->isRButtonDown())
	{
		if (!m_mouseDown)
		{
			addModel();
		}
		m_mouseDown = true;
	}
	else if (_getInputSystem()->isRButtonUp())
	{
		m_mouseDown = false;
	}
}
/*
*/
void cTest::updateKeyboard()
{
	if (_getInputSystem()->isJustPressedKey(DIK_1))
	{
		cEntityModel* entity = _getSceneMgr()->getEntity(_T("jang.bmd_0"));
		if (entity)
			entity->setMotion(ANI_IDLE);
	}
	else if (_getInputSystem()->isJustPressedKey(DIK_2))
	{
		cEntityModel* entity = _getSceneMgr()->getEntity(_T("jang.bmd_0"));
		if (entity)
			entity->setMotion(ANI_RUN);
	}
	else if (_getInputSystem()->isJustPressedKey(DIK_3))
	{
		cEntityModel* entity = _getSceneMgr()->getEntity(_T("jang.bmd_0"));
		if (entity)
			entity->setMotion(ANI_WALK);
	}
	else if (_getInputSystem()->isJustPressedKey(DIK_4))
	{
		cEntityModel* entity = _getSceneMgr()->getEntity(_T("jang.bmd_0"));
		if (entity)
			entity->setMotion(ANI_ATTACK);
	}
	else if (_getInputSystem()->isJustPressedKey(DIK_5))
	{
		cEntityModel* entity = _getSceneMgr()->getEntity(_T("jang.bmd_0"));
		if (entity)
			entity->setMotion(ANI_DEAD);
	}
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
/*
*/
void cTest::addModel()
{
	static int count = 0;

	_getSceneMgr()->pickTerrain();
	sVector3 const& pickPos = _getPick()->getPickPos();

	std_string const& filename = m_testModelList.getFileName();
	BUID _buid = _getStringHelper()->makeFilenameToBUID(filename);
	std_string buid = _getStringHelper()->make(_T("%s_%d"), _buid.c_str(), count++);

	sMsgCreateEntity* msg = new sMsgCreateEntity;
	msg->m_filename = filename;
	msg->m_buid = buid;
	msg->m_pos = pickPos;
	msg->m_radian = 0.0f;
	msg->m_loadingType = 0;
	msg->m_shareType = 0;
	_getMsgMgr()->addBucket(msg);
}