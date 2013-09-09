
#include "BreadShadowManager.h"
#include "BreadProjectedShadow.h"
#include "BreadShadowVolume.h"
#include "BreadShadowMap.h"
#include "BreadShadowPSSM.h"
#include "BreadShadowCSM.h"
#include "BreadCore.h"
#include "BreadGui.h"


namespace Bread
{
	std_string cShadowManager::DEFAULT_SHADOW = _T("shadowmap");

	/*
	*	
	*/
	cShadowManager::cShadowManager() : m_shadow(NULL)
	{
	}
	/*
	*	
	*/
	cShadowManager::~cShadowManager()
	{
		safeDelete(m_shadow);
		m_objFactory.unregisterCallBackAll();
	}
	/*
	*/
	void cShadowManager::registerObjectCallBack()
	{
		m_objFactory.registerCallBack(_T("projected"),		createShadowProjected);
		m_objFactory.registerCallBack(_T("shadowmap"),		createShadowMap);
		m_objFactory.registerCallBack(_T("shadowvolume"),	createShadowVolume);
		m_objFactory.registerCallBack(_T("pssm"),			createShadowPSSM);
		m_objFactory.registerCallBack(_T("csm"),			createShadowCSM);
	}
	/*
	*	
	*/
	bool cShadowManager::initialize(std_string const& type, uint width, uint height)
	{
		registerObjectCallBack();

		assert(!m_shadow);
		m_type = type;
		m_shadow = createShadow(type);
		if (m_shadow)
			m_shadow->initialize(width, height);
		return true;
	}
	/*
	*	
	*/
	cShadowBase* cShadowManager::createShadow(std_string const& type)
	{
		cShadowBase* object = m_objFactory.createObject(type);
		trace(_T("invalid create shadow type"));
		return object;
	}
	/*
	*	
	*/
	void cShadowManager::update(float elapsedtime)
	{
		if (m_shadow)
			m_shadow->update(elapsedtime);
	}
	/*
	*	
	*/
	void cShadowManager::render()
	{
		if (m_shadow)
			m_shadow->render();
	}
	/*
	*	
	*/
	void cShadowManager::beforeRender3D()
	{
		if (m_shadow)
			m_shadow->beforeRender3D();
	}
	/*
	*	
	*/
	void cShadowManager::afterRender3D()
	{
		if (m_shadow)
			m_shadow->afterRender3D();
	}
	/*
	*	
	*/
	void cShadowManager::beforeRender2D()
	{
		if (m_shadow)
			m_shadow->beforeRender2D();
	}
	/*
	*	
	*/
	void cShadowManager::afterRender2D()
	{
		if (m_shadow)
			m_shadow->afterRender2D();
	}
	/*
	*	
	*/
	int cShadowManager::renderDebugInfo(int y)
	{
		if (!m_shadow)
			return y;
		y += 16;

		_getFontMgr()->drawD3DText(10, y+=16, _T("[Shadow]"), cD3DFont::HEIGHT_14, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		return m_shadow->renderDebugInfo(y);
	}
}



