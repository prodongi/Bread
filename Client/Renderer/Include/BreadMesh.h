
#ifndef _BreadMesh_h_
#define _BreadMesh_h_

#include "BreadDeclaration.h"
#include "BreadBounding.h"

namespace Bread
{
	struct sVBInfo;
	struct sIBInfo;

	/*
	*/
	class cMesh
	{
	public:
		cMesh();
		~cMesh();
		void copy(cMesh& copyData, uint shareType);

	public:
		/// @brief m_buid는 mesh의 이름과 같은 값이다
		BUID m_buid;
		BUID m_mtlBuid;
		BUID m_vbBuid;
		BUID m_ibBuid;
		sCullFigure	m_cullFigure;
		/* 쓰레드로 로딩한 데이타를merge하기위해 갖고 있는 임시 버퍼 
		*/
		sVBInfo* m_vbInfoBuffer;
		sIBInfo* m_ibInfoBuffer;
	};
}









#endif