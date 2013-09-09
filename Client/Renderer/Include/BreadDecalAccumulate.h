
#ifndef _BreadDecalAccumulate_h_
#define _BreadDecalAccumulate_h_

/*
	BreadDecalAccumulate.h
	Copyright (c) prodongi. All rights resserved.

	date : 2012-12-18
	purpose : 
*/

#include "BreadBucketVector.h"

namespace Bread
{
	class cRenderData;
	class cMaterial;
	class cMesh;
	
	/*
	*/
	struct sDecalAccumulateData
	{
		sDecalAccumulateData(cMaterial const* mtl, cMesh* mesh, cRenderData* rd) : m_mtl(mtl), m_mesh(mesh), m_rd(rd)
		{}

		cMaterial const* m_mtl;
		cMesh* m_mesh;
		cRenderData* m_rd;
	};

	/*
	*/
	class cDecalAccumulate
	{
	public:
		cDecalAccumulate();
		~cDecalAccumulate();
		void clear();
		void collect(cRenderData* renderData, cMaterial* mtl);

	private:
		enum {	CONTAINER_SIZE = 100	};
		cBucketVector<sDecalAccumulateData, CONTAINER_SIZE> m_list;
	};
}

#endif