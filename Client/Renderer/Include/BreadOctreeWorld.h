
#ifndef _BreadOctreeWorld_h_
#define _BreadOctreeWorld_h_

#include "BreadBucketVector.h"
#include "BreadOctree.h"

namespace Bread
{
	/*
		@date 2012.
		@auth prodongi
		@desc 월드 옥트리를 관리하는 클래스,
			  scene 파일에서 읽어드리는 scene이 여러개일 수 있다.
			  m_worldScene는 scene파일 밖에 있는 것들을 관리한다(구름 같은 것들)
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
		/// @brief m_sceneList중에 메인 scene octree
		cOctree* m_mainScene;
	};
}



#endif