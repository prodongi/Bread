
#include "BreadMesh.h"
#include "BreadVBManager.h"
#include "BreadIBManager.h"

namespace Bread
{
	/*
	 *	
	 */
	cMesh::cMesh() : m_vbInfoBuffer(NULL), m_ibInfoBuffer(NULL)
	{
	}
	/*
	 *	
	 */
	cMesh::~cMesh()
	{
		safeDelete(m_vbInfoBuffer);
		safeDelete(m_ibInfoBuffer);
	}
	/*
	*/
	void cMesh::copy(cMesh& copyData, uint /*shareType*/)
	{
		copyData.m_buid = m_buid;
		copyData.m_mtlBuid = m_mtlBuid;
		copyData.m_vbBuid = m_vbBuid;
		copyData.m_ibBuid = m_ibBuid;
		m_cullFigure.copy(copyData.m_cullFigure);
	}
}















