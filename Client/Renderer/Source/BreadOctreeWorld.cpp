
#include "BreadOctreeWorld.h"
#include "BreadBounding.h"
#include "BreadRenderer.h"
#include "BreadProfile.h"
#include "BreadOctreeNode.h"

namespace Bread
{
	/*
	*/
	cOctreeWorld::cOctreeWorld() : m_worldScene(NULL), m_mainScene(NULL)
	{
	}
	/*
	*/
	cOctreeWorld::~cOctreeWorld()
	{
	}
	/*
	*/
	bool cOctreeWorld::initialize(float width, float height, float depth)
	{
		initWorldScene(width, height, depth);

		return true;
	}
	/*
	*/
	void cOctreeWorld::initWorldScene(float width, float height, float depth)
	{
		assert(!m_worldScene);

		sBox box;
		box.set(-width, width, -height, height, -depth, depth);
		m_worldScene = new cOctree;
		m_worldScene->createRoot(&box);
	}
	/*
	*/
	void cOctreeWorld::finalize()
	{
		safeDelete(m_worldScene);
	}
	/*
	*/
	void cOctreeWorld::addScene(cOctree* sceneOctree, bool main)
	{
		if (!sceneOctree)
			return ;
		m_sceneList.pushBack(&sceneOctree);
		if (main)
			m_mainScene = sceneOctree;
	}
	/*
	*/
	bool cOctreeWorld::add(cRenderData* renderData, sBox const* box, bool isSceneObject, int maxDepth)
	{
		struct sAddScene
		{
			sAddScene(cRenderData* _renderData, sBox const* _box, bool _isSceneObject, int _maxDepth, bool* _added)
			{
				renderData = _renderData;
				box = _box;
				isSceneObject = _isSceneObject;
				maxDepth = _maxDepth;
				added = _added;
			}
			void operator ()(cOctree* octree)
			{
				if (*added)
					return ;
				if (octree->add(renderData, box, isSceneObject, maxDepth))
					*added = true;
			}
			cRenderData* renderData;
			sBox const* box;
			bool isSceneObject;
			int maxDepth;
			bool* added;
		};
		/// 먼저 scenelist에서 찾는다
		bool added = false;
		m_sceneList.forEach(sAddScene(renderData, box, isSceneObject, maxDepth, &added));

		/// worldOctree maxDepth는 1이다.
		if (!added)
			m_worldScene->add(renderData, box, isSceneObject, 1);

		return true;
	}
	/*
	*/
	void cOctreeWorld::culling(cFrustum const* frustum)
	{
		struct sCulling
		{
			sCulling(cFrustum const* _frustum)
			{
				frustum = _frustum;
			}
			void operator ()(cOctree* octree)
			{
				octree->culling(frustum);
			}
			cFrustum const* frustum;
		};
		{
			PROFILE(_T("sceneList forEach"));
			m_sceneList.forEach(sCulling(frustum));
		}
		{
			PROFILE(_T("worldScene culling"));
			m_worldScene->culling(frustum);
		}
	}
	/*
	*/
	void cOctreeWorld::renderOctree(int depth)
	{
		struct sRender
		{
			sRender(int _depth) : depth(_depth) {}
			void operator ()(cOctree* octree)
			{
				cRenderer::renderOctreeNode(octree->getRoot(), depth);
			}
			int depth;
		};
		m_sceneList.forEach(sRender(depth));
		cRenderer::renderOctreeNode(m_worldScene->getRoot(), depth);
	}
	/*
	*/
	sBox const* cOctreeWorld::getWorldBound() const
	{
		return m_worldScene->getRoot()->getBound();
	}
}