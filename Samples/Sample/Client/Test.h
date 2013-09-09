
#ifndef _Test_h_
#define _Test_h_

#include "Shader.h"
#include "BreadRenderToTexture.h"
#include "BreadWindowRenderData.h"
#include "BreadFileHelper.h"
#include "BreadTextureManager.h"

namespace Bread
{
	class cDepthTextureShader;
	class cSSAOShader;
}

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
	propertyConstFunc(int, CurRenderRtt, m_curRenderRtt)

private:
	bool initShader();
	bool initRtt();
	bool initEntity();
	void updateMouse();
	void updateKeyboard();

public:
	enum	{	RTT_BECKMANN, RTT_LIGHTMAP, RTT_NUM	};

private:
	/// 
	cSkinShader* m_shader;
	cRenderToTextureNAA* m_rtt;
	std_string m_normalMap;
	int m_curRenderRtt;
};







#endif