
#ifndef _Test_h_
#define _Test_h_

/*
*/
class cTest
{
public:
	cTest();
	~cTest();
	bool initialize();
	void update(float elapsedtime);
	int renderDebugInfo(int y);
	void beforeRender3D();
	void afterRender3D();
	void beforeRender2D();
	void afterRender2D();

private:
	bool initEntity();
	void updateMouse();
	void updateKeyboard();
};







#endif