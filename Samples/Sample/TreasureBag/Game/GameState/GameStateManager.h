
#ifndef _GameStateManager_h_
#define _GameStateManager_h_

/*
	BreadGameState.h
	Copyright (c) prodongi. All rights resserved.

	date : 2012-08-09
	purpose : 
*/

namespace Bread
{
	class cStateManager;
}

#define _getGameStateMgr()	cGameStateManager::getInstance()

class cGameStateManager
{
	declareInstance(cGameStateManager)

public:
	cGameStateManager();
	~cGameStateManager();

	bool initialize();
	void finalize();
	void update(float elapsedtime);
	/*
		"login"
		"game"
		"loading"
	*/
	void setState(char const* stateName, float delay = 0.0f, bool flush = false);
	void changeState(char const* stateName, float delay = 0.0f, bool flush = false);

private:
	void registerState();

private:
	cStateManager* m_stateMgr;

};

#endif