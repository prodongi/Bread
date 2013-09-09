
#include "BreadDebugRenderManager.h"
#include "BreadRenderer.h"
#include "BreadTexture.h"

namespace Bread
{
	implementInstance(cDebugRenderManager)

	/*
	*/
	cDebugRenderManager::cDebugRenderManager()
	{
		assignInstance(this)
	}
	/*
	*/
	cDebugRenderManager::~cDebugRenderManager()
	{
		assignInstance(NULL)
	}
	/*
	*/
	void cDebugRenderManager::initialize()
	{
	}
	/*
	*/
	void cDebugRenderManager::finalize()
	{
		clearList();
	}
	/*
	*/
	void cDebugRenderManager::clearList()
	{
		m_bucketScreen.clear();
	}
	/*
	*/
	void cDebugRenderManager::render3D()
	{
	}
	/*
	*/
	void cDebugRenderManager::render2D()
	{
		renderScreen();
	}
	/*
	*/
	void cDebugRenderManager::renderScreen()
	{
		if (0 == m_bucketScreen.getBucketSize())
			return ;

		for (m_bucketScreen.begin(); !m_bucketScreen.isEnd(); m_bucketScreen.next())
		{
			sScreen const& s = m_bucketScreen.getIterData();
			cRenderer::renderDebugScreen<sUiPlane>(s.m_sx, s.m_sy, s.m_w, s.m_h, s.m_tex);
		}
	}
	/*
	*/
	void cDebugRenderManager::addScreen(float sx, float sy, float w, float h, cTexture* tex)
	{
		sScreen s;
		s.m_sx = sx;
		s.m_sy = sy;
		s.m_w = w;
		s.m_h = h;
		s.m_tex = tex;
		m_bucketScreen.pushBack(s);
	}
}