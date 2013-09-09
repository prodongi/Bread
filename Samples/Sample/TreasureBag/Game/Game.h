
#ifndef _Game_h_
#define _Game_h_

#include "BreadNavigation.h"

class cGameStateManager;

/*
*/
class cGame
{
public:
	cGame();
	~cGame();
	bool initialize();
	void finalize();
	void update(float elapsedtime);
	int renderDebugInfo(int y);
	void beforeRender3D();
	void afterRender3D();
	void beforeRender2D();
	void afterRender2D();
	void mouseEventCallBack();

private:
	bool initEntity();
	bool initGameStateMgr();
	void updateMouse();
	void updateKeyboard();

private:
	cGameStateManager* m_gameStateMgr;
	sPathResult m_pathResult;
};







#endif