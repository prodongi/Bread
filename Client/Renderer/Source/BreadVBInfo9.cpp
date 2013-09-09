
#include "BreadVBInfo.h"
#include "BreadD3DSystemClassify.h"
#include "BreadRenderer.h"
#include "BreadStringHelper.h"

namespace Bread
{
	/*
	*/
	void setFVFData(sFVFInfo& fvfInfo, int& count, int& offset, int elementSize)
	{
		++count;
		fvfInfo.m_is = true;
		fvfInfo.m_offset = offset;
		offset += elementSize;
	}
	/*
	*/
	void setD3DVertexElement9(LPD3DVERTEXELEMENT9 element, WORD stream, int offset, BYTE type, BYTE method, BYTE usage, BYTE usageIndex)
	{
		element->Stream		= stream;
		element->Offset			= (WORD)offset;
		element->Type			= type;
		element->Method		= method;
		element->Usage			= usage;
		element->UsageIndex	= usageIndex;
	}

	/*
	*/
	sVBInfo::~sVBInfo()
	{
		m_vb.release();
		safeDeleteArray(m_localJointResult);
		safeDeleteArray(m_element);
		safeRelease(m_decl);
	}
	/*
	*/
	void sVBInfo::setFlag(bool dynamic)
	{
		if (dynamic)
		{
			m_usage = D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY;
			m_pool = D3DPOOL_DEFAULT;
			m_lock = D3DLOCK_DISCARD | D3DLOCK_READONLY;
		}
		else
		{
			m_usage = D3DUSAGE_WRITEONLY;
			m_pool = D3DPOOL_MANAGED;
			m_lock = 0;
		}
	}
	/*
	*/
	bool sVBInfo::setFVFInfoToElement(int elementNum)
	{
		m_elementNum = elementNum;
		m_element = new D3DVERTEXELEMENT9[elementNum+1];

		int c = 0;
		for (int fvf_n = 0; fvf_n < sFVFInfo::FVF_NUM; ++fvf_n)
		{
			if (!m_fvfInfo[fvf_n].m_is)
				continue;

			int offset = m_fvfInfo[fvf_n].m_offset;
			LPD3DVERTEXELEMENT9 element = m_element + c;

			switch (fvf_n)
			{
			case sFVFInfo::FVF_XYZ:			
				setD3DVertexElement9(element, 0, offset, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0);	
				break;
			case sFVFInfo::FVF_XYZRHW:
				setD3DVertexElement9(element, 0, offset, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0);
				break;
			case sFVFInfo::FVF_XYZB4:
				setD3DVertexElement9(element, 0, offset, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0);
				++c;
				setD3DVertexElement9(element+1, 0, offset, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0);
				break;
			case sFVFInfo::FVF_LASTBETA_UBYE4:
				setD3DVertexElement9(element, 0, offset, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0);
				break;
			case sFVFInfo::FVF_NORMAL:
				setD3DVertexElement9(element, 0, offset, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0);
				break;
			case sFVFInfo::FVF_TANGENT:	
				setD3DVertexElement9(element, 0, offset, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0);
				break;
			case sFVFInfo::FVF_BINORMAL:	
				setD3DVertexElement9(element, 0, offset, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0);
				break;
			case sFVFInfo::FVF_COLOR:	
				setD3DVertexElement9(element, 0, offset, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0);
				break;
			case sFVFInfo::FVF_TEX1:
				if (1 == m_fvfInfo[fvf_n].m_reserved0)
					setD3DVertexElement9(element, 0, offset, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0);
				else
					setD3DVertexElement9(element, 0, offset, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0);
				break;
			case sFVFInfo::FVF_TEX2:	
				setD3DVertexElement9(element, 0, offset, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1);
				break;
			}

			++c;
		}

		setD3DVertexElement9(m_element + c, 0xff, 0, D3DDECLTYPE_UNUSED, 0, 0, 0);

		if (FAILED(cD3DSystem::getD3DDevice()->CreateVertexDeclaration(m_element, &m_decl)))
			return false;
		return true;
	}
	/*
	*/
	bool sVBInfo::setD3DFVFToElement(DWORD fvf)
	{
		for (int fvf_n = 0; fvf_n < sFVFInfo::FVF_NUM; ++fvf_n)
		{
			m_fvfInfo[fvf_n].m_is = false;
		}

		int offset = 0;
		int count = 0;
		if (D3DFVF_XYZ & fvf)			setFVFData(m_fvfInfo[sFVFInfo::FVF_XYZ], count, offset, sizeof (float) * 3);
		if (D3DFVF_XYZRHW & fvf)		setFVFData(m_fvfInfo[sFVFInfo::FVF_XYZRHW], count, offset, sizeof (float) * 4);
		if (D3DFVF_XYZB4 & fvf)			setFVFData(m_fvfInfo[sFVFInfo::FVF_XYZB4], count, offset, sizeof (float) * 3);
		if (D3DFVF_LASTBETA_UBYTE4 & fvf)		setFVFData(m_fvfInfo[sFVFInfo::FVF_LASTBETA_UBYE4], count, offset, sizeof (byte) * 4);
		if (D3DFVF_NORMAL & fvf)		setFVFData(m_fvfInfo[sFVFInfo::FVF_NORMAL], count, offset, sizeof (float) * 3);
		//if (D3DFVF_TANGENT & fvf)	setFVFData(m_fvfInfo[sFVFInfo::FVF_TANGENT], count, sizeof (float) * 3);
		//if (D3DFVF_BINORMAL & fvf)	setFVFData(m_fvfInfo[sFVFInfo::FVF_BINORMAL], count, sizeof (float) * 3);
		if (D3DFVF_DIFFUSE & fvf)		setFVFData(m_fvfInfo[sFVFInfo::FVF_COLOR], count, offset, sizeof (DWORD));
		if (D3DFVF_TEX0 & fvf)			setFVFData(m_fvfInfo[sFVFInfo::FVF_TEX0], count, offset, sizeof (float) * 2);
		if (D3DFVF_TEX1 & fvf)		
		{
			if (D3DFVF_TEXCOORDSIZE3(0) & fvf)
			{
				m_fvfInfo[sFVFInfo::FVF_TEX1].m_reserved0 = 1;
				setFVFData(m_fvfInfo[sFVFInfo::FVF_TEX1], count, offset, sizeof (float) * 3);
			}
			else
				setFVFData(m_fvfInfo[sFVFInfo::FVF_TEX1], count, offset, sizeof (float) * 2);
		}
		if (D3DFVF_TEX2 & fvf)			setFVFData(m_fvfInfo[sFVFInfo::FVF_TEX2], count, offset, sizeof (float) * 2);

		return setFVFInfoToElement(count);
	}
	/*
	*/
	bool sVBInfo::lock(void** data)
	{
		return m_vb.lock(0, m_vbSize, data, m_lock);
	}
	/*
	*/
	bool sVBInfo::unlock()
	{
		return m_vb.unlock();
	}
	/*
	*/
	void sVBInfo::setLocalJointResult(int localIndex, sMatrix4 const& mat)
	{
		if (0 > localIndex)
		{
			assert(0 && _T("invalid local joint setResult : 0 > localIndex"));
			return ;
		}
		if ((int)getLocalJointNum() >= localIndex)
		{
			assert(0 && _T("invalid local joint setResult : m_num >= localIndex"));
			return ;
		}
		m_localJointResult[localIndex] = mat;
	}

	/*
	*/
	sVBCacheInfo::~sVBCacheInfo()
	{
		safeDeleteArray(m_buffer);
		safeDeleteArray(m_element);
		safeRelease(m_decl);
	}
	/*
	*/
	void sVBCacheInfo::setFlag(bool dynamic)
	{
		if (dynamic)
		{
			m_usage = D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY;
			m_pool = D3DPOOL_DEFAULT;
			m_lock = D3DLOCK_DISCARD | D3DLOCK_READONLY;
		}
		else
		{
			m_usage = D3DUSAGE_WRITEONLY;
			m_pool = D3DPOOL_MANAGED;
			m_lock = 0;
		}
	}
	/*
	*/
	bool sVBCacheInfo::copy(sVBInfo* copyData, uint shareType)
	{
		copyData->m_vbSize = m_bufferSize;
		copyData->m_stride = m_stride;
		copyData->m_usage = m_usage;
		copyData->m_pool = m_pool;
		copyData->m_lock = m_lock;
		copyData->m_vertexNum = m_vertexNum;
		copyData->m_elementNum = m_elementNum;
		memcpy(copyData->m_fvfInfo, m_fvfInfo, sFVFInfo::FVF_NUM * sizeof (sFVFInfo));
		copyData->m_primitiveType = m_primitiveType;
		if (ST_VB & shareType)
		{
			copyData->m_buid = m_buid;
		}
		else
		{
			copyData->m_buid = _getStringHelper()->make(_T("%s_%d"), m_buid.c_str(), _getVBMgr()->getCount());
		}

		if (m_localJointIndex.getJointNum())
		{
			copyData->m_localJointIndex = m_localJointIndex;
			copyData->m_localJointResult = new sMatrix4[m_localJointIndex.getJointNum()];
		}

		copyData->m_element = new D3DVERTEXELEMENT9[m_elementNum + 1];
		memcpy(copyData->m_element, m_element, (m_elementNum + 1) * sizeof (D3DVERTEXELEMENT9));
		if (!checkHResultReturn(cD3DSystem::getD3DDevice()->CreateVertexDeclaration(m_element, &copyData->m_decl)))
			return false;

		if (!_getVBMgr()->create(copyData, m_buffer))
			return false;

		return true;
	}
	/*
	*/
	bool sVBCacheInfo::setFVFInfoToElement(int elementNum)
	{
		m_elementNum = elementNum;
		m_element = new D3DVERTEXELEMENT9[elementNum+1];

		int c = 0;
		for (int fvf_n = 0; fvf_n < sFVFInfo::FVF_NUM; ++fvf_n)
		{
			if (!m_fvfInfo[fvf_n].m_is)
				continue;

			int offset = m_fvfInfo[fvf_n].m_offset;
			LPD3DVERTEXELEMENT9 element = m_element + c;

			switch (fvf_n)
			{
			case sFVFInfo::FVF_XYZ:			
				setD3DVertexElement9(element, 0, offset, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0);	
				break;
			case sFVFInfo::FVF_XYZRHW:
				setD3DVertexElement9(element, 0, offset, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0);
				break;
			case sFVFInfo::FVF_XYZB4:
				setD3DVertexElement9(element, 0, offset, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0);
				++c;
				setD3DVertexElement9(element+1, 0, offset+12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0);
				break;
			case sFVFInfo::FVF_LASTBETA_UBYE4:
				setD3DVertexElement9(element, 0, offset, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0);
				break;
			case sFVFInfo::FVF_NORMAL:
				setD3DVertexElement9(element, 0, offset, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0);
				break;
			case sFVFInfo::FVF_TANGENT:	
				setD3DVertexElement9(element, 0, offset, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0);
				break;
			case sFVFInfo::FVF_BINORMAL:	
				setD3DVertexElement9(element, 0, offset, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0);
				break;
			case sFVFInfo::FVF_COLOR:	
				setD3DVertexElement9(element, 0, offset, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0);
				break;
			case sFVFInfo::FVF_TEX0:	
				setD3DVertexElement9(element, 0, offset, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0);
				break;
			case sFVFInfo::FVF_TEX1:	
				setD3DVertexElement9(element, 0, offset, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1);
				break;
			case sFVFInfo::FVF_TEX2:
				setD3DVertexElement9(element, 0, offset, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2);
				break;			
			}

			++c;
		}

		setD3DVertexElement9(m_element + c, 0xff, 0, D3DDECLTYPE_UNUSED, 0, 0, 0);

		return true;
	}
}