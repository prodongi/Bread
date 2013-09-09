
#ifndef _BreadBatchRenderManager_h_
#define _BreadBatchRenderManager_h_

#include "BreadDeclaration.h"
#include "BreadBucketVector.h"

#define _getBatchRenderMgr() Bread::cBatchRenderManager::getInstance()

namespace Bread
{
	struct sLineVertex;
	struct sVector3;
	struct sBox;
	struct sSphere;
	class cRenderData;

	/*
		@date 2011.07.11
		@auth prodongi
		@desc 老褒贸府 皋聪历
		@todo 
	*/
	class cBatchRenderManager
	{
		declareInstance(cBatchRenderManager)
		declareClassCreater(cBatchRenderManager)

	public:
		struct sEntityList
		{
			sEntityList() : m_rd(NULL) {}
			sEntityList(cRenderData* rd) : m_rd(rd)	{}
			cRenderData* m_rd;
		};
		enum	{	MASK_ALL, MASK_SCENE, MASK_NOT_SCENE, MASK_NUM	};
		enum	{	CONTAINER_SIZE = 100	};

		typedef std::vector<cBatchRenderManager::sEntityList>	vecEntityList;
		typedef cBucketVector<cBatchRenderManager::sEntityList, CONTAINER_SIZE> bucket_rd;

	public:
		virtual void initialize();
		virtual void finalize();
		virtual void update(float elapsedtime);
		virtual void render3D();
		virtual void render2D();
		
		void clearList();
		void push(sLineVertex const* vertexList, unsigned int num);
		void push(sVector3 const* v1, sVector3 const* v2, DWORD color);
		void push(sVector3 const& v1, sVector3 const& v2, DWORD color);
		void push(std::vector<sVector3> const& vertexList, DWORD color);
		void push(sBox const* box, DWORD color);
		void push(sSphere const* sphere, DWORD color);
		void pushAtNormal(sVector3 const* ori, sVector3 const* normal, float normalScale, DWORD color);
		void pushLineStrip(sLineVertex const* lineVertex, unsigned int num);
		void push(cRenderData* rd, bool isSceneObject);
		void renderEntityCullFigure(uint mask = MASK_ALL);
		int getEntityListSize(uint mask = MASK_ALL) const;

		/// @biref mask(MASK_ALL, MASK_SCENE, MASK_NOT_SCENE)
		template <typename T> void extract(T& t, uint mask = MASK_ALL)
		{
			if (!isValidEntityList(mask))
				return ;
			t.extract(m_entityList[mask]);
		}
		//
		template <typename Fn> void forEachEntityList(Fn fn, uint mask = MASK_ALL)
		{
			if (!isValidEntityList(mask))
				return ;
			m_entityList[mask]->forEach(fn);
		}

	protected:
		virtual void initPrimitiveLineList();
		virtual void initPrimitiveLineStrip();
		void renderLine();
		void renderLineList();
		void renderLineStrip();
		bool isValidEntityList(uint mask) const;
		
	protected:
		sLineVertex* m_lineList;
		unsigned int m_primitiveLineListCount;

		sLineVertex* m_lineStrip;
		unsigned int m_primitiveLineStripCount;

		static unsigned int m_lineListNum;
		static unsigned int m_lineStripNum;

		bucket_rd* m_entityList[MASK_NUM];			/// @brief total list
	};
}



#endif