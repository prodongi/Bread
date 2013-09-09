
#ifndef _SampleApp_h_
#define _SampleApp_h_

#include <assert.h>
#include "Test.h"

/*
 *	
 */
class cSampleApplication : public Bread::cApplication
{
	declareInstance(cSampleApplication)

public:
	cSampleApplication() : m_camAngle(0.0f)
	{
		assignInstance(this)
	}
	virtual ~cSampleApplication()
	{
		assignInstance(NULL)
	}
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
	void createShader();
	void initDebugFlag();
	void initCamera();
	void initLight();
	bool initUi();
	void initPostProcess();
	bool loadFont();
	bool initTest();
	virtual void beforeRender3D();
	virtual void afterRender3D();
	virtual void beforeRender2D();
	virtual void afterRender2D();

public:
	float m_camAngle;

private:
	cTest m_test;
};

















#endif