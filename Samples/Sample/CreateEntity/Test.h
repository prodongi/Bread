
#ifndef _Test_h_
#define _Test_h_

struct sModelList
{
	~sModelList();
	void init();
	std_string const& getFileName(int index = -1);
	std::vector<std_string> m_fileList;
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
	void addModel();

private:
	sModelList m_testModelList;
	bool m_mouseDown;
};







#endif