
#ifndef _TreasureApp_h_
#define _TreasureApp_h_

#include "BreadApplication.h"

class cGame;

/*
 *	
 */
class cTreasureApplication : public cApplication
{
	declareInstance(cTreasureApplication)

public:
	cTreasureApplication();
	virtual ~cTreasureApplication();
	virtual bool initApp();
	virtual void initException();
	virtual void update(float elapsedtime);
	virtual int renderDebugInfo(int y);
	virtual LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (!cApplication::wndProc(hWnd, msg, wParam, lParam))
			return 0;

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
protected:
	virtual void render();
	void createShader();
	void initDebugFlag();
	void initCamera();
	void initLight();
	bool initUi();
	void initPostProcess();
	bool loadFont();
	bool initGame();
	void initState();
	virtual void mouseEventCallBack();
	virtual void beforeRender3D();
	virtual void afterRender3D();
	virtual void beforeRender2D();
	virtual void afterRender2D();

public:
	float m_camAngle;

private:
	cGame* m_game;
};

















#endif