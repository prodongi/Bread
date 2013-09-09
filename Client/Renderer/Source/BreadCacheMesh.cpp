
#include "BreadCacheMesh.h"
#include "BreadVBManager.h"
#include "BreadIBManager.h"

namespace Bread
{
	/*
	*/
	cCacheMesh::cCacheMesh() : m_vbInfo(NULL), m_ibInfo(NULL)
	{
	}
	/*
	*/
	cCacheMesh::~cCacheMesh()
	{
		safeDelete(m_vbInfo);
		safeDelete(m_ibInfo);
	}
	/*
	*/
	void cCacheMesh::copy(cMesh& copyData, uint shareType)
	{
		copyData.m_buid = m_buid;
		copyData.m_mtlBuid = m_mtlBuid;
		m_cullFigure.copy(copyData.m_cullFigure);

		/* vb
		*/
		if (shareType & ST_VB)
		{
			if (_getVBMgr()->is(m_vbInfo->m_buid))
			{
				copyData.m_vbBuid = m_vbInfo->m_buid;
			}
			else
			{
				copyData.m_vbInfoBuffer = new sVBInfo;
				if (m_vbInfo->copy(copyData.m_vbInfoBuffer, shareType))
				{
					copyData.m_vbBuid = copyData.m_vbInfoBuffer->m_buid;
					copyData.m_vbInfoBuffer = NULL;
				}
			}
		}
		else
		{
			copyData.m_vbInfoBuffer = new sVBInfo;
			if (m_vbInfo->copy(copyData.m_vbInfoBuffer, shareType))
			{
				copyData.m_vbBuid = copyData.m_vbInfoBuffer->m_buid;
				copyData.m_vbInfoBuffer = NULL;
			}
		}

		/* ib
		*/
		if (ST_IB & shareType)
		{
			if (_getIBMgr()->is(m_ibInfo->m_buid))
			{
				copyData.m_ibBuid = m_ibInfo->m_buid;
			}
			else
			{
				copyData.m_ibInfoBuffer = new sIBInfo;
				if (m_ibInfo->copy(copyData.m_ibInfoBuffer, shareType))
				{
					copyData.m_ibBuid = copyData.m_ibInfoBuffer->m_buid;
					copyData.m_ibInfoBuffer = NULL;
				}
			}
		}
		else
		{
			copyData.m_ibInfoBuffer = new sIBInfo;
			if (m_ibInfo->copy(copyData.m_ibInfoBuffer, shareType))
			{
				copyData.m_ibBuid = copyData.m_ibInfoBuffer->m_buid;
				copyData.m_ibInfoBuffer = NULL;
			}
		}
	}
}