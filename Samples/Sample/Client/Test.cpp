
#include "stdafx.h"
#include "Test.h"
#include "BreadSceneManager.h"
#include "BreadMaterial.h"
#include "BreadShaderList.h"
#include "BreadSceneManager.h"
#include "BreadGui.h"
#include "BreadPick.h"
#include "BreadMsgManager.h"
#include "BreadAnimationActionEnum.h"
#include "BreadDebugRenderManager.h"
#include "BreadEntityPlane.h"
#include "BreadRenderData.h"

/*
 *	
 */
cTest::cTest() : m_shader(NULL), m_rtt(NULL), m_curRenderRtt(NULL)
{
}
/*
 *	
 */
cTest::~cTest()
{
	safeDelete(m_shader);
	safeDeleteArray(m_rtt);
}
/*
 *	
 */
bool cTest::initialize()
{
	if (!initEntity())	return false;
	if (!initShader())	return false;
	if (!initRtt())		return false;

	// 아바타의 노멀맵 파일을 저장
	cEntityModel* avatar = _getEntityMgr()->get(_T("avatar"));
	if (avatar)
	{
		cRenderData* rd = _getSceneMgr()->getRenderData(avatar->getRenderDataBuid());
		std::vector<BUID> bumpMapList;
		rd->getMapBuidList(cMaterial::BUMP, bumpMapList, true);
		m_normalMap = bumpMapList[0];
		bumpMapList.clear();
	}
	
	return true;
}
/*
*/
bool cTest::initEntity()
{
	_getSceneMgr()->createScene(_T("plane.bsd"), 0);
	//_getSceneMgr()->createScene(_T("human.bsd"), 0);

	//_getSceneMgr()->createEntity<cEntityModel>(_T("Data\\scene1\\plane.bmd"), _T("box"), sVector3::ZERO, 0.0f, 0);
/*
	filename = _T("BaseData\\scenegrid.bsd");
	buid = _T("a");
	_getSceneMgr()->createEntity<cEntityModel>(filename, buid, sVector3::ZERO, 0.0f, 0);

	TCHAR temp[NAME_LEN];
	for (int i = 0; i < 10; ++i)
	{
		_stprintf(temp, _T("%d"), i);
		sVector3 pos((float)(rand()%500), 0.0f, (float)(rand()%500));
		//sVector3 pos(0.0f, 0.0f, 0.0f);//sVector3::ZERO);
		if (0 == i)
			_getSceneMgr()->createEntity<cEntityAvatar>(_T("data\\vagary\\vagary.bmd"), _T("avatar"), sVector3(0.0f, 0.0f, 0.0f), 0.0f);
		else
			_getSceneMgr()->createEntity<cEntityMover>(_T("data\\vagary\\vagary.bmd"/*"data\\horse.bmd"*//*), temp, pos, 0.0f);
	}

	// camera target
	_getSceneMgr()->createEntity<cEntityModel>(_T("data\\lighttarget.bmd"), _T("lighttarget"), sVector3::ZERO, 0.0f);
	*/
	return true;
}
/*
*/
bool cTest::initShader()
{
	m_shader = new cSkinShader;
	if (!m_shader->createEffect(_T("skinshading.fx"), cD3DSystem::getD3DDevice(), cShader::makeFlags()))
		return false;
	return true;
}
/*
*/
bool cTest::initRtt()
{
	m_rtt = new cRenderToTextureNAA[RTT_NUM];
	if (!m_rtt[RTT_BECKMANN].initialize(0, 512, 512, false))	return false;
	if (!m_rtt[RTT_LIGHTMAP].initialize(0, 512, 512, false))	return false;
	return true;
}
/*
 *	
 */
void cTest::update(float elapsedtime)
{
	updateMouse();
	updateKeyboard();

	unreferencedParameter(elapsedtime);
}
/*
*/
void cTest::updateMouse()
{
}
/*
*/
void cTest::updateKeyboard()
{
}
/*
 *	
 */
int cTest::renderDebugInfo(int y)
{
	return y;
}
/*
 *	
 */
void cTest::beforeRender3D()
{
}
/*
 *	
 */
void cTest::afterRender3D()
{
}
/*
 *	
 */
void cTest::beforeRender2D()
{
	/*
	static bool renderBeckmann = false;
	if (!renderBeckmann)
	{
		renderBeckmann = true;
		m_shader->setTechnique(_T("TBeckmann"));
		cRenderer::renderScreen(&m_rtt[RTT_BECKMANN], m_shader, m_rtt[RTT_BECKMANN].getScreen(), true);
	}

	m_shader->setTechnique(_T("TLightMap"));
	m_shader->setTexture(_T("normalTexture"), _getTextureMgr()->getTexture(m_normalMap));
	cRenderer::renderScreen(&m_rtt[RTT_LIGHTMAP], m_shader, m_rtt[RTT_LIGHTMAP].getScreen(), true);
	*/
}
/*
 *	
 */
void cTest::afterRender2D()
{
	//_getDebugRenderMgr()->addScreen(0.0f, 400.0f, 128.0f, 128.0f, m_rtt[RTT_BECKMANN].getTexture());
	//_getDebugRenderMgr()->addScreen(0.0f, 550.0f, 128.0f, 128.0f, m_rtt[RTT_LIGHTMAP].getTexture());
}