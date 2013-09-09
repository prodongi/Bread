
#include "stdafx.h"
#include "BreadDebugRenderManager.h"
#include "BreadPostProcessManager.h"
#include "BreadSceneManager.h"
#include "GameStateManager.h"
#include "BreadTerrain.h"
#include "BreadPick.h"
#include "BreadGui.h"
#include "Game.h"
/*
 *	
 */
cGame::cGame() : m_gameStateMgr(NULL)
{
}
/*
 *	
 */
cGame::~cGame()
{
}
/*
 *	
 */
bool cGame::initialize()
{
	if (!initEntity())
		return false;
	if (!initGameStateMgr())
		return false;

	return true;
}
/*
*/
void cGame::finalize()
{
	if (m_gameStateMgr)
	{
		m_gameStateMgr->finalize();
		safeDelete(m_gameStateMgr);
	}
}
/*
*/
bool cGame::initEntity()
{
	return true;
}
/*
*/
bool cGame::initGameStateMgr()
{
	assert(!m_gameStateMgr);
	m_gameStateMgr = new cGameStateManager;
	return m_gameStateMgr->initialize();
}
/*
 *	
 */
void cGame::update(float elapsedtime)
{
	unreferencedParameter(elapsedtime);

	updateMouse();
	updateKeyboard();

	if (m_gameStateMgr)
		m_gameStateMgr->update(elapsedtime);

	//
	//_getBatchRenderMgr()->push(m_pathResult.m_path, 0xffffffff);
}
/*
*/
void cGame::updateMouse()
{
}
/*
*/
void cGame::updateKeyboard()
{
}
/*
*/
void cGame::mouseEventCallBack()
{
	if (_getInputSystem()->isJustRButtonDown())
	{
		/*
		static int count = 0;

		_getSceneMgr()->pickTerrain();
		sVector3 const& pickPos = _getPick()->getPickPos();

		std_string const& filename = _T("skeleton.bmd");
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
		*/

		if (_getSceneMgr()->pickTerrain())
		{
			int moveType;
			float speed;
			if (rand()%2 == 0)
			{
				moveType = ENTITY_MOVE_WALK;
				speed = 70.0f;
			}
			else
			{
				moveType = ENTITY_MOVE_RUN;
				speed = 300.0f;
			}

			m_pathResult.clear();
			cEntity* avatar = _getEntityMgr()->getAvatar();
			bool ret = _getTerrain()->pathFind(avatar->getPosition(), avatar->getCellIndex(),
									_getPick()->getPickPos(), _getPick()->getPickCellIndex(), &m_pathResult);
			if (!ret)
				return ;
									

			sMsgMove* msg = new sMsgMove;
			msg->m_buid = _getEntityMgr()->getAvatarBuid();
			msg->m_path = m_pathResult.m_path;//_getPick()->getPickPos();
			msg->m_moveType = moveType;
			msg->m_speed = speed;
			_getMsgMgr()->addBucket(msg);
		}
	}
}
/*
 *	
 */
int cGame::renderDebugInfo(int y)
{
	return y;
}
/*
 *	
 */
void cGame::beforeRender3D()
{
}
/*
 *	
 */
void cGame::afterRender3D()
{
	_getDebugRenderMgr()->addScreen(140.0f, 550.0f, 128.0f, 128.0f, _getPostProcessMgr()->getHelperTexture(cPostProcessManager::HELPER_DEPTH));
}
/*
 *	
 */
void cGame::beforeRender2D()
{
}
/*
 *	
 */
void cGame::afterRender2D()
{
}