
#ifndef _Test_h_
#define _Test_h_

struct sTextureCache
{
	~sTextureCache();
	void init();
	void update(float elapsedtime);

	std::vector<std_string> m_fileList;
	sUpdateTime<float> m_update;
};

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

private:
	sTextureCache m_textureCache;
};







#endif