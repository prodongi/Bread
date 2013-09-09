
#ifndef _BreadOctreeWorld_h_
#define _BreadOctreeWorld_h_

#include "BreadBucketVector.h"
#include "BreadOctree.h"

namespace Bread
{
	/*
		@date 2012.
		@auth prodongi
		@desc ���� ��Ʈ���� �����ϴ� Ŭ����,
			  scene ���Ͽ��� �о�帮�� scene�� �������� �� �ִ�.
			  m_worldScene�� scene���� �ۿ� �ִ� �͵��� �����Ѵ�(���� ���� �͵�)
		@todo 
	*/
	class cOctreeWorld
	{
	public:
		cOctreeWorld();
		~cOctreeWorld();

		bool initialize(float width, float height, float depth);
		void finalize();
		void addScene(cOctree* sceneOctree, bool main);
		bool add(cRenderData* renderData, sBox const* box, bool isSceneObject, int maxDepth = -1);
		void culling(cFrustum const* frustum);	
		void renderOctree(int depth);
		sBox const* getWorldBound() const;
		cOctree* getMain() const	{	return m_mainScene;	}
		
	private:
		void initWorldScene(float width, float height, float depth);

	private:
		cOctree* m_worldScene;
		cBucketVector<cOctree*> m_sceneList;
		/// @brief m_sceneList�߿� ���� scene octree
		cOctree* m_mainScene;
	};
}



#endif