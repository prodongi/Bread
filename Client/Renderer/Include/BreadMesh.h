
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
		/// @brief m_buid�� mesh�� �̸��� ���� ���̴�
		BUID m_buid;
		BUID m_mtlBuid;
		BUID m_vbBuid;
		BUID m_ibBuid;
		sCullFigure	m_cullFigure;
		/* ������� �ε��� ����Ÿ��merge�ϱ����� ���� �ִ� �ӽ� ���� 
		*/
		sVBInfo* m_vbInfoBuffer;
		sIBInfo* m_ibInfoBuffer;
	};
}









#endif