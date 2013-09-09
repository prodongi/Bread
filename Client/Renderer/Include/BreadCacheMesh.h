
#ifndef _BreadCacheMesh_h_
#define _BreadCacheMesh_h_

#include "BreadDeclaration.h"
#include "BreadBounding.h"
#include "BreadMesh.h"

namespace Bread
{
	struct sVBCacheInfo;
	struct sIBCacheInfo;

	/*
	*/
	class cCacheMesh
	{
	public:
		cCacheMesh();
		~cCacheMesh();
		void copy(cMesh& copyData, uint shareType);

	public:
		/// @brief m_buid는 mesh의 이름과 같은 값이다
		BUID m_buid;
		BUID m_mtlBuid;
		sVBCacheInfo* m_vbInfo;
		sIBCacheInfo* m_ibInfo;
		sCullFigure	m_cullFigure;
	};
}









#endif