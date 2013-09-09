
#include "stdafx.h"
#include "EntityAvatar.h"
#include "BreadSceneManager.h"
#include "BreadGui.h"
#include "BreadPick.h"

/*
/*
 *	
 */
cEntityAvatarTest::cEntityAvatarTest()
{
}
/*
 *	
 */
cEntityAvatarTest::~cEntityAvatarTest()
{
}
/*
 *	
 */
void cEntityAvatarTest::update(float elapsedtime)
{
	cEntityMoverTest::update(elapsedtime);
	if (_getInputSystem()->isRButtonDown())
	{
		_getSceneMgr()->pickTerrain();
		setDestPos(_getPick()->getPickPos());
		setMoving(true);
	}
}