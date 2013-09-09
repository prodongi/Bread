
#include "BreadAnimationActionManager.h"
#include "BreadStringHelper.h"
#include "BreadAnimationActionEnum.h"
#include "BreadAnimationIdle.h"
#include "BreadAnimationWalk.h"
#include "BreadAnimationRun.h"
#include "BreadAnimationAttack.h"
#include "BreadAnimationDead.h"
#include "BreadCore.h"

namespace Bread
{
	implementInstance(cAnimationActionManager)

	char const* cAnimationActionManager::ANIMATION_ACTION_LUA_TABLE_NAME = "AnimationAction";

	/*
	*/
	cAnimationActionManager::cAnimationActionManager()
	{
		assignInstance(this)
	}
	/*
	*/
	cAnimationActionManager::~cAnimationActionManager()
	{
		assignInstance(NULL)
	}
	/*
	*/
	void cAnimationActionManager::initialize()
	{
		registAction();
	}
	/*
	*/
	void cAnimationActionManager::registAction()
	{
		size_t tableSize = _getLuaMgr()->getn(ANIMATION_ACTION_LUA_TABLE_NAME);
		assert(tableSize && _T("animationAction table size is zero"));

		int type;
		char const* name;
		TCHAR tname[128];
		for (size_t tableIdx = 0; tableIdx < tableSize; ++tableIdx)
		{
			type = _getLuaMgr()->getTableValue<int>(ANIMATION_ACTION_LUA_TABLE_NAME, tableIdx, "type");
			name = _getLuaMgr()->getTableValue<char const*>(ANIMATION_ACTION_LUA_TABLE_NAME, tableIdx, "name");

			cAnimationAction* action = createAction(type);
			if (NULL == action)
				continue;

			_getStringHelper()->m2w(name, tname, 128);
			insert(tname, type, action);			
		}
	}
	/*
	*/
	cAnimationAction* cAnimationActionManager::createAction(int type)
	{
		switch (type)
		{
		case ANI_IDLE:	return new cAnimationIdle;
		case ANI_WALK:	return new cAnimationWalk;
		case ANI_RUN:	return new cAnimationRun;
		case ANI_ATTACK:return new cAnimationAttack;
		case ANI_DEAD:	return new cAnimationDead;
		}

		return NULL;
	}
	/*
	*/
	void cAnimationActionManager::finalize()
	{
		m_idTable.clear();
	}
	/*
	*/
	BUID const& cAnimationActionManager::getBuid(uint id)
	{
		return m_idTable.get(id);
	}
	/*
	*/
	void cAnimationActionManager::insert(BUID const& buid, uint id, cAnimationAction* enityMotion)
	{
		if (!enityMotion)
		{
			assert(!enityMotion);
			return ;
		}
		enityMotion->setBuid(buid);
		cBucketHash::insert(buid, &enityMotion);

		m_idTable.insert(id, buid);
	}
}